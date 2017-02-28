#include "MHNG_math.h"

namespace MHNG_math {

    ImageMatrixGrayScale rgbToGray(const ImageMatrix& image){
        ImageMatrixGrayScale gray_image(image.size(), ImageRowPixelsGRAY(image.begin()->size()));
        int v_size = (int)image.size();
        int h_size = (int)image[0].size();
        int gx = 0;
        int gy = 0;
        int sum = 0;

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
//              std::cout<< sum << std::endl;
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
    // TODO: CONTOUR NOT AFFECTED!!
    MHNG_SeamCarve getSeamCarveVertical(const ImageMatrixGrayScale& image) {
        MHNG_SeamCarve carve(image[0].size(), MHNG_PixelLocation());
        ImageMatrixGrayScale lookup_path = image;

        //TODO: gray_image = inverted image
        ImageMatrixGrayScale lookup_value = image;
        for (int j = 0; j < image[0].size(); ++j) {
            lookup_path[0][j] = 99999999999;
            lookup_value[0][j] = image[0][j];
        }

        // lookup_path = 1 - go [i-1][j-1] , lookup_path = 2 - go [i][j-1], lookup_path = 3 - go [i+1][j-1]
        for (int i = 1; i < image.size() - 1; ++i) {
            for (int j = 1; j < image[0].size() - 1; ++j) {
                int prev_hor = j - 1;
                if (image[i][j] + lookup_value[i][prev_hor] < image[i][j] + lookup_value[i-1][prev_hor]){
                    if (image[i][j] + lookup_value[i][prev_hor] <= image[i][j] + lookup_value[i+1][prev_hor]) {
                        lookup_path[i][j] = 2;
                        lookup_value[i][j] = image[i][j] + lookup_value[i][prev_hor];
                    } else {
                        lookup_path[i][j] = 3;
                        lookup_value[i][j] = image[i][j] + lookup_value[i+1][prev_hor];
                    }
                } else {
                    if (image[i][j] + lookup_value[i-1][prev_hor] < image[i][j] + lookup_value[i+1][prev_hor]) {
                        lookup_path[i][j] = 1;
                        lookup_value[i][j] = image[i][j] + lookup_value[i-1][prev_hor];
                    } else {
                        lookup_path[i][j] = 3;
                        lookup_value[i][j] = image[i][j] + lookup_value[i+1][prev_hor];
                    }
                }
            }
        }
        int carve_start = 1;
        int min_val = std::numeric_limits<int>::max();
        int width = image.size();
        int height = image[0].size();
        for (int k = 1; k < width - 1 ; ++k) {
            if (lookup_value[k][height - 1] < min_val) {
                carve_start = k;
                min_val = lookup_value[k][height - 1];
            }
        }
        int location_h = carve_start;
        int location_v = height - 1;
        carve[0].x = location_h;
        carve[0].y = location_v;
        for (int l = 0; l < width ; ++l) {
        }
        for (int k = 1; k < height ; ++k) {
            if (lookup_path[location_h][location_v] == 1) {
                --location_h;
            } else if (lookup_path[location_h][location_v] == 3) {
                ++location_h;
            }
            --location_v;
            carve[k].x = location_h;
            carve[k].y = location_v;
        }
        return carve;
    }

    ImageMatrixGrayScale removeCarveVertical(const ImageMatrixGrayScale& image, MHNG_SeamCarve carve_vertical) {

        ImageMatrixGrayScale resized_gray_image(image.size()-1, ImageRowPixelsGRAY(image.begin()->size()));

        int height = image[0].size();
        int shifted = 0;
        for (int j = 0; j < height; ++j) {
            shifted = 0;
            for (int i = 0; i < image.size()-1; ++i) {
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
}
