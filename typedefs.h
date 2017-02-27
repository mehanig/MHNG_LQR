#ifndef LQR_TYPEDEFS_H_H
#define LQR_TYPEDEFS_H_H
#include <iostream>
#include <vector>

typedef size_t MHNG_PIXELSIZE;
typedef unsigned char byte;

struct MHNGN_Pixel {
    MHNG_PIXELSIZE R;
    MHNG_PIXELSIZE G;
    MHNG_PIXELSIZE B;
};

typedef size_t MHNG_Pixel_GRAYSCALE;

typedef std::vector<MHNGN_Pixel> ImageRowPixels;
typedef std::vector<ImageRowPixels> ImageMatrix;

typedef std::vector<MHNG_Pixel_GRAYSCALE> ImageRowPixelsGRAY;
typedef std::vector<ImageRowPixelsGRAY> ImageMatrixGrayScale;

class MHNG_Frame {

};




#endif //LQR_TYPEDEFS_H_H
