#ifndef LQR_TYPEDEFS_H_H
#define LQR_TYPEDEFS_H_H
#include <iostream>
#include <vector>

typedef size_t MHNG_PIXELSIZE;
typedef unsigned char byte;

typedef unsigned long long int ull;

struct MHNGN_Pixel {
    MHNG_PIXELSIZE R;
    MHNG_PIXELSIZE G;
    MHNG_PIXELSIZE B;
};

struct MHNG_PixelLocation {
    int x;
    int y;
    MHNG_PixelLocation(): x(0), y(0) {};
};

typedef std::vector<MHNG_PixelLocation> MHNG_SeamCarve;

typedef size_t MHNG_Pixel_GRAYSCALE;

typedef std::vector<MHNGN_Pixel> ImageRowPixels;
typedef std::vector<ImageRowPixels> ImageMatrix;

typedef std::vector<MHNG_Pixel_GRAYSCALE> ImageRowPixelsGRAY;
typedef std::vector<ImageRowPixelsGRAY> ImageMatrixGrayScale;

typedef std::vector<ull> LookUpRow;
typedef std::vector<LookUpRow> LookUpTable;
typedef std::array<std::vector<std::vector<ull>>, 2> LookUpData;


class MHNG_Frame {

};




#endif //LQR_TYPEDEFS_H_H
