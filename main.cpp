#include <iostream>

typedef size_t MHNG_PIXELSIZE;

struct MHNGN_Pixel {
    MHNG_PIXELSIZE R;
    MHNG_PIXELSIZE G;
    MHNG_PIXELSIZE B;
};

class MHNG_Frame {

};

int main() {
    std::cout << "Test Build" << std::endl;
    MHNGN_Pixel pixel = {100,200,300};
    MHNGN_Pixel * ppixel = new MHNGN_Pixel();

    for (int j = 0; j <= 100; j++){
        std::unique_ptr<MHNGN_Pixel> ppixel_tmp = std::make_unique<MHNGN_Pixel>();
        ppixel_tmp->R = 1000;
    }
    ppixel->R = 10;
    ppixel->G = 10;
    ppixel->B = 10;

    return 0;
}