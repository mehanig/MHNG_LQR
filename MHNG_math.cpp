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
//                std::cout<< sum << std::endl;
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
}
