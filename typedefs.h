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


typedef std::vector<MHNGN_Pixel> ImageRowPixels;
typedef std::vector<ImageRowPixels> ImageMatrix;

class MHNG_Frame {

};


#endif //LQR_TYPEDEFS_H_H
