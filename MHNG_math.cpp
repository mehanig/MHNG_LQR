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
        ull lookup_value[width][height];
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                lookup_path[i][j] = 0;
                lookup_value[i][j] = image[i][j];
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

        for (int k = 0; k < height ; ++k) {
            ull min_val = std::numeric_limits<int>::max();
            for (int j = 0; j < width ; ++j) {
                if (lookup_value[j][k] < min_val) {
                    min_val = lookup_value[j][k];
                }
            }
        }

        return carve;
    }

    ImageMatrixGrayScale removeCarveVertical(const ImageMatrixGrayScale& image, MHNG_SeamCarve carve_vertical) {

        ull height = image[0].size();
        ull width = image.size();
        ImageMatrixGrayScale resized_gray_image(width-1, ImageRowPixelsGRAY(height));
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
//        for (int j = 0; j < height; ++j) {
//            for (int i = carve_vertical[j].x; i < width-1; ++i) {
//                image[i][j] = image[i+1][j];
//            }
//        }
        int shifted = 0;
        auto tmp1 = image;
        ImageMatrix tmp = tmp1;
        for (int j = 0; j < height; ++j) {
            shifted = 0;
            for (int i = 0; i < width-1; ++i) {
                if (carve_vertical[j].x == i ) {
                    shifted = 1;
                }
                if (shifted == 1) {
                    image[i][j] = tmp[i+1][j];
                } else {
                    image[i][j] = tmp[i][j];
                }
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
        for (int i = 0; i < remove_val; ++i) {
            auto seam = getSeamCarveVertical(tmp);
            tmp = removeCarveVertical(gray, seam);
            gray = tmp;
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
