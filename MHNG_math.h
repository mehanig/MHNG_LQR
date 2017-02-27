#ifndef LQR_MHNG_MATH_H
#define LQR_MHNG_MATH_H

#include "typedefs.h"

namespace MHNG_math {
    ImageMatrixGrayScale rgbToGray(const ImageMatrix& image);
    ImageMatrix grayToRgb(const ImageMatrixGrayScale& gray_image);
    ImageMatrixGrayScale sobelFilter(const ImageMatrix& image);
    int xGradient(ImageMatrixGrayScale& image, int x, int y);
    int yGradient(ImageMatrixGrayScale& image, int x, int y);
}

#endif //LQR_MHNG_MATH_H
