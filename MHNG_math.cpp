#include <array>
#include <vector>
#include "MHNG_math.h"

namespace MHNG_math {

    ImageMatrixGrayScale rgbToGray(const ImageMatrix& image){
        ull v_size = (int)image.size();
        ull h_size = (int)image[0].size();
        ImageMatrixGrayScale gray_image(v_size, ImageRowPixelsGRAY(h_size));

        for (int i = 0; i < v_size; ++i) {
            for (int j = 0; j < h_size; ++j) {
                gray_image[i][j] = (image[i][j].R + image[i][j].B + image[i][j].G) / 3;
            }
        }
        return gray_image;
    }

    ImageMatrix grayToRgb(const ImageMatrixGrayScale& gray_image){
        ImageMatrix image(gray_image.size(), ImageRowPixels(gray_image.begin()->size()));
        int v_size = (int)image.size();
        int h_size = (int)image[0].size();
        for (int i = 0; i < v_size; ++i) {
            for (int j = 0; j < h_size; ++j) {
                image[i][j].R = gray_image[i][j];
                image[i][j].G = gray_image[i][j];
                image[i][j].B = gray_image[i][j];
            }
        }
        return image;
    }


    ImageMatrixGrayScale sobelFilter(const ImageMatrix& image) {
        ImageMatrixGrayScale gray_image = rgbToGray(image);
        int v_size = (int)image.size();
        int h_size = (int)image[0].size();
        int gx = 0;
        int gy = 0;
        int sum = 0;

        // TODO: BORDERS NOT AFFECTED!!
        ImageMatrixGrayScale tmp_image = gray_image;
        for (int i = 1; i < v_size-1; ++i) {
            for (int j = 1; j < h_size-1; ++j) {
                gx = xGradient(tmp_image, i, j);
                gy = yGradient(tmp_image, i, j);
                sum = abs(gx) + abs(gy);
                sum = sum > 255 ? 255:sum;
                sum = sum < 0 ? 0 : sum;
                gray_image[i][j] = sum;
            }
        }
        return gray_image;
    }

    int xGradient(ImageMatrixGrayScale& image, int x, int y)
    {
        return (int)(image[x+1][y-1] + 2 * image[x+1][y] + image[x+1][y+1] - image[x-1][y-1] - 2*image[x-1][y] - image[x-1][y+1]);
    }

    int yGradient(ImageMatrixGrayScale& image, int x, int y)
    {
        return (int)(image[x-1][y+1] + 2*image[x][y+1] + image[x+1][y+1] - image[x-1][y-1] - 2* image[x][y-1] - image[x+1][y-1]);
    }

    // TODO: MIGHT BE OVERFLOW !!! LARGE SUMM!
    MHNG_SeamCarve getSeamCarveVertical(const ImageMatrixGrayScale& image) {
        ull width = image.size();
        ull height = image[0].size();
        MHNG_SeamCarve carve(height, MHNG_PixelLocation());
        int lookup_path[width][height];
        auto lookup_value = image;
//        for (int i = 0; i < width; i++) {
//            for (int j = 0; j < height; j++) {
//                lookup_path[i][j] = 0;
//            }
//        }


        // lookup_path = 1 - go [i-1][j-1] , lookup_path = 2 - go [i][j-1], lookup_path = 3 - go [i+1][j-1]
        for (int j = 1; j < height; ++j) {
            for (int i = 0; i < width; ++i) {
                int prev_hor = j - 1;
                ull up = lookup_value[i][j] + lookup_value[i][prev_hor];
                ull left;
                if (i > 0) { left = lookup_value[i][j] + lookup_value[i-1][prev_hor]; } else {left = up;}
                ull right;
                if (i < width-1) { right = lookup_value[i][j] + lookup_value[i+1][prev_hor]; } else { right = up;}
                ull best_way = std::min({up, left, right});
                lookup_value[i][j] = best_way;
                if (best_way == up) {
                    lookup_path[i][j] = 2;
                } else if (best_way == left) {
                    lookup_path[i][j] = 1;
                } else {
                    lookup_path[i][j] = 3;
                }
            }
        }
        int carve_start = 0;
        ull min_val = lookup_value[0][height - 1];
        for (int k = 0; k < width ; ++k) {
            if (lookup_value[k][height - 1] < min_val) {
                carve_start = k;
                min_val = lookup_value[k][height - 1];
            }
        }
        int location_h = carve_start;
        int location_v = height - 1;
        carve[height-1].x = location_h;
        carve[height-1].y = location_v;
        for (int k = 1; k < height ; ++k) {
            if (lookup_path[location_h][location_v] == 1) {
                --location_h;
            } else if (lookup_path[location_h][location_v] == 3) {
                ++location_h;
            }
            --location_v;
            carve[height-k-1].x = location_h;
            carve[height-k-1].y = location_v;
        }

        return carve;
    }

    MHNG_SeamCarve getSeamCarveVerticalFromCache(const ImageMatrixGrayScale& image, const LookUpData& lookup_data) {
        ull width = image.size();
        ull height = image[0].size();
//        std::cout << width << " IN getSeamCarveVerticalFromCache" <<" " << lookup_data[0].size() << "\n";
        MHNG_SeamCarve carve(height, MHNG_PixelLocation());
        auto lookup_path = lookup_data[0];
        auto lookup_value = lookup_data[1];

        int carve_start = 0;
        ull min_val = lookup_value[0][height - 1];
        for (int k = 0; k < width ; ++k) {
            if (lookup_value[k][height - 1] < min_val) {
                carve_start = k;
                min_val = lookup_value[k][height - 1];
            }
        }
        int location_h = carve_start;
        int location_v = height - 1;
        carve[height-1].x = location_h;
        carve[height-1].y = location_v;
        for (int k = 1; k < height ; ++k) {
            if (lookup_path[location_h][location_v] == 1) {
                --location_h;
            } else if (lookup_path[location_h][location_v] == 3) {
                ++location_h;
            }
            --location_v;
            carve[height-k-1].x = location_h;
            carve[height-k-1].y = location_v;
        }
//        std::cout << carve[height-1].x << std::endl;
//        std::cout << width << "OUT OF getSeamCarveVerticalFromCache" << "\n";
        return carve;
    }

    void UpdateCacheAfterCarving(LookUpData& cache, const MHNG_SeamCarve &carve) {
        // Lookup_path edits
        ull height = cache[0][0].size();
        ull width = cache[0].size();

        for (int j = 0; j < height; ++j) {
            for (int i = carve[j].x; i < width-1; ++i) {
                cache[0][i][j] = cache[0][i+1][j];
                cache[1][i][j] = cache[1][i+1][j];
                cache[2][i][j] = cache[2][i+1][j];
//                raw_value[i][j] = raw_value[i+1][j];
            }
        }
        cache[0].pop_back();
        cache[1].pop_back();
        cache[2].pop_back();
//        raw_value.pop_back();

        std::vector<int> buffer;
        std::vector<int> new_buffer;
        int curr_carve_x = carve[0].x;
        int delta = 1;
        int new_width = (int)width -1 ;
        for (int j = 1; j < height; j++) {
            int left_bound = std::max({curr_carve_x - delta, 0});
            int right_bound = std::min({curr_carve_x + delta, new_width});
//            std::cout << left_bound << " " << right_bound << " " << width << "\n";
            for (int i = left_bound; i < right_bound; i++) {
                int prev_hor = j - 1;
                ull up = cache[2][i][j] + cache[1][i][prev_hor];
                ull left;
                if (i > 0) { left = cache[2][i][j] + cache[1][i-1][prev_hor]; } else {left = up;}
                ull right;
                if (i < new_width - 1) { right = cache[2][i][j] + cache[1][i+1][prev_hor]; } else { right = up;}
                ull best_way = std::min({up, left, right});
                cache[1][i][j] = best_way;
                if (best_way == up) {
                    cache[0][i][j] = 2;
                } else if (best_way == left) {
                    cache[0][i][j] = 1;
                } else {
                    cache[0][i][j] = 3;
                }
            }
            delta = 20;
            curr_carve_x = carve[j].x;
        }
    }

    LookUpData CacheLookupPath(const ImageMatrixGrayScale& image) {
        ull width = image.size();
        ull height = image[0].size();
        LookUpData data;

        LookUpTable lookup_path = LookUpTable(width, LookUpRow(height));
        LookUpTable lookup_value = LookUpTable(width, LookUpRow(height));
        LookUpTable lookup_raw_value = LookUpTable(width, LookUpRow(height));
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                lookup_path[i][j] = 0;
                lookup_value[i][j] = image[i][j];
                lookup_raw_value[i][j] = image[i][j];
            }
        }

        // lookup_path = 1 - go [i-1][j-1] , lookup_path = 2 - go [i][j-1], lookup_path = 3 - go [i+1][j-1]
        for (int j = 1; j < height; ++j) {
            for (int i = 0; i < width; ++i) {
                int prev_hor = j - 1;
                ull up = lookup_value[i][j] + lookup_value[i][prev_hor];
                ull left;
                if (i > 0) { left = lookup_value[i][j] + lookup_value[i-1][prev_hor]; } else {left = up;}
                ull right;
                if (i < width-1) { right = lookup_value[i][j] + lookup_value[i+1][prev_hor]; } else { right = up;}
                ull best_way = std::min({up, left, right});
                lookup_value[i][j] = best_way;
                if (best_way == up) {
                    lookup_path[i][j] = 2;
                } else if (best_way == left) {
                    lookup_path[i][j] = 1;
                } else {
                    lookup_path[i][j] = 3;
                }
            }
        }
        data[0] = lookup_path;
        data[1] = lookup_value;
        data[2] = lookup_raw_value;
        return data;
    }

    //TODO: REWRITE TO BE SHORT!
    void UpdateLookupPathFromCarve(LookUpData& data, MHNG_SeamCarve& carve_vertical) {
        auto image = data[0];
        ull height = image[0].size();
        ull width = image.size();

        for (int j = 0; j < height; ++j) {
            for (int i = carve_vertical[j].x; i < width-1; ++i) {
                image[i][j] = image[i+1][j];
            }
        }
        image.pop_back();

        image = data[1];

        for (int j = 0; j < height; ++j) {
            for (int i = carve_vertical[j].x; i < width-1; ++i) {
                image[i][j] = image[i+1][j];
            }
        }
        image.pop_back();

    }

    ImageMatrixGrayScale removeCarveVertical(const ImageMatrixGrayScale& image, MHNG_SeamCarve carve_vertical) {
        auto resized_image = image;
        ull height = resized_image[0].size();
        ull width = resized_image.size();
        for (int j = 0; j < height; ++j) {
            for (int i = carve_vertical[j].x; i < width-1; ++i) {
                resized_image[i][j] = resized_image[i+1][j];
            }
        }
        return resized_image;
    }

    void removeCarveVerticalWithShift(ImageMatrixGrayScale& image, MHNG_SeamCarve carve_vertical) {
        ull height = image[0].size();
        ull width = image.size();
        for (int j = 0; j < height; ++j) {
            for (int i = carve_vertical[j].x; i < width-1; ++i) {
                image[i][j] = image[i+1][j];
            }
        }
        image.pop_back();
    }

    ImageMatrix removeCarveVerticalRGB(const ImageMatrix& image, MHNG_SeamCarve carve_vertical) {

        ull height = image[0].size();
        ull width = image.size();
        ImageMatrix resized_gray_image(width-1, ImageRowPixels(height));
        int shifted = 0;
        for (int j = 0; j < height; ++j) {
            shifted = 0;
            for (int i = 0; i < width-1; ++i) {
                if (carve_vertical[j].x == i ) {
                    shifted = 1;
                }
                if (shifted == 1) {
                    resized_gray_image[i][j] = image[i+1][j];
                } else {
                    resized_gray_image[i][j] = image[i][j];
                }
            }
        }
        return resized_gray_image;
    }


    int removeCarveVerticalInPlace(ImageMatrix& image, MHNG_SeamCarve& carve_vertical) {
        ull height = image[0].size();
        ull width = image.size();
        for (int j = 0; j < height; ++j) {
            for (int i = carve_vertical[j].x; i < width-1; ++i) {
                image[i][j] = image[i+1][j];
            }
        }
        image.pop_back();
        return 0;
    }

    int resizeLQRHorisontalInPlace(ImageMatrix& image, int remove_val) {
        ImageMatrixGrayScale gray = sobelFilter(image);
        std::vector<MHNG_SeamCarve> carves_list;
//        MHNG_SeamCarve carves_list[remove_val];
        ImageMatrixGrayScale tmp = sobelFilter(image);

        auto cache = CacheLookupPath(gray);
        LookUpData new_cache;
        for (int i = 0; i < remove_val; ++i) {
            auto seam = getSeamCarveVerticalFromCache(gray, cache);
            UpdateCacheAfterCarving(cache, seam);
            removeCarveVerticalWithShift(gray, seam);
            carves_list.push_back(seam);
        }
        for (int i = 0; i < remove_val; ++i) {
            removeCarveVerticalInPlace(image, carves_list[i]);
        }
        return 0;
    }



    ImageMatrixGrayScale resizeLQRHorisontal(const ImageMatrixGrayScale& image, int remove_val) {
        ImageMatrixGrayScale rescaled = image;
        ImageMatrixGrayScale tmp = rescaled;
        for (int i = 0; i < remove_val; ++i) {
            auto seam = getSeamCarveVertical(tmp);
            tmp = removeCarveVertical(rescaled, seam);
            rescaled = tmp;
        }
        return rescaled;
    }


    ImageMatrix resizeLQRHorisontalRGB(const ImageMatrix& image, int remove_val) {
        ImageMatrix rescaled = image;
        ImageMatrix tmp = rescaled;

        for (int i = 0; i < remove_val; ++i) {
            auto seam = getSeamCarveVertical(rgbToGray(tmp));
            tmp = removeCarveVerticalRGB(rescaled, seam);
            rescaled = tmp;
        }
        return rescaled;
    }
}
