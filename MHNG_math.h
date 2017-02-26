#ifndef LQR_MHNG_MATH_H
#define LQR_MHNG_MATH_H

#include "typedefs.h"

namespace MHNG_math {
    size_t rgbToGray();

    size_t sobelFilter(ImageMatrix& image);
}

#endif //LQR_MHNG_MATH_H
