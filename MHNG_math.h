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

    LookUpData CacheLookupPath(const ImageMatrixGrayScale& image);
    void UpdateLookupPathFromCarve(LookUpData& data, const MHNG_SeamCarve&);
    void UpdateCacheAfterCarving(LookUpData &cache, const MHNG_SeamCarve &carve);
    MHNG_SeamCarve getSeamCarveVerticalFromCache(const ImageMatrixGrayScale& image, const LookUpData& lookup_data);
    ImageMatrixGrayScale removeCarveVertical(const ImageMatrixGrayScale& image, MHNG_SeamCarve carve_vertical);
    void removeCarveVerticalWithShift(ImageMatrixGrayScale& image, MHNG_SeamCarve carve_vertical);

    int removeCarveVerticalInPlace(ImageMatrix& image, MHNG_SeamCarve& carve_vertical);
    int resizeLQRHorisontalInPlace(ImageMatrix& image, int remove_val);

    ImageMatrix removeCarveVerticalRGB(const ImageMatrix& image, MHNG_SeamCarve carve_vertical);
    ImageMatrix resizeLQRHorisontalRGB(const ImageMatrix& image, int remove_val);
}

#endif //LQR_MHNG_MATH_H
