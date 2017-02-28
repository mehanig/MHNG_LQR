#ifndef LQR_MHNG_MATH_H
#define LQR_MHNG_MATH_H

#include "typedefs.h"
#include <limits>

namespace MHNG_math {
    ImageMatrixGrayScale rgbToGray(const ImageMatrix& image);
    ImageMatrix grayToRgb(const ImageMatrixGrayScale& gray_image);

    ImageMatrixGrayScale sobelFilter(const ImageMatrix& image);

    int xGradient(ImageMatrixGrayScale& image, int x, int y);
    int yGradient(ImageMatrixGrayScale& image, int x, int y);

    MHNG_SeamCarve getSeamCarveVertical(const ImageMatrixGrayScale& image);
    ImageMatrixGrayScale removeCarve(const ImageMatrixGrayScale& imageMatrixGrayScale, MHNG_SeamCarve carve);
    ImageMatrixGrayScale resizeLQRHorisontal(const ImageMatrixGrayScale& image, int remove_val);
}

#endif //LQR_MHNG_MATH_H
