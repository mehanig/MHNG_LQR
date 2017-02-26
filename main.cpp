#include <iostream>
#include <array>
#include <fstream>
#include <string>
#include "typedefs.h"
#include "MHNG_math.h"
#include "file_operations.h"

std::string file_in = "./infile.bmp";
std::string file_out = "./outfile.bmp";

const int WIDTH = 512;
const int HEIGHT = 512;
int rgb_size = HEIGHT * WIDTH * 3;


char* BMPHEADER = new char[54];

//*******
// READ BMP FOR TESTING ONLY 24 BITS
//*******

int ReadBMP(std::string filename, unsigned char* data) {
    int i;
    FILE* f = fopen(filename.c_str(), "rb");

    if(f == NULL)
        throw "Argument Exception";

    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header
    for (int k = 0; k < 54; ++k) {
        BMPHEADER[k] = info[k];
    }

    int width = *(int*)&info[18];
    int height = *(int*)&info[22];

    std::cout << std::endl;
    std::cout << "  Name: " << filename << std::endl;
    std::cout << " Width: " << width << std::endl;
    std::cout << "Height: " << height << std::endl;

    int row_padded = (width*3 + 3) & (~3);
    unsigned char * tmp_row = new unsigned char[row_padded];
    unsigned char tmp;

    for(int i = 0; i < height; i++) {
        fread(tmp_row, sizeof(unsigned char), row_padded, f);
        for(int j = 0; j < width*3; j += 3) {
            // Convert (B, G, R) to (R, G, B)
            tmp = tmp_row[j];
            tmp_row[j] = tmp_row[j+2];
            tmp_row[j+2] = tmp;
            data[(i*width*3) + j] = tmp_row[j+2];
            data[(i*width*3) + j+1] = tmp_row[j+1];
            data[(i*width*3) + j+2] = tmp_row[j];
        }
    }
    std::cout << " WAS " << (int)data[783477] << std::endl;
    fclose(f);
    return width * height * 3;
}


int WriteBMP(std::string filename, unsigned char* data) {
    std::fstream f = std::fstream("./outfile.bmp", std::ios::out | std::ios::binary);
    f.write(BMPHEADER, 54);
    int width = *(int*)&BMPHEADER[18];
    int height = *(int*)&BMPHEADER[22];
    std::cout << std::endl;
    std::cout << " Width: " << width << std::endl;
    std::cout << "Height: " << height << std::endl;
    std::cout << " NICE " << (int)data[783477] << std::endl;
    char * readydata = reinterpret_cast<char*>(data);
    f.write(readydata, width * height * 3);
    f.close();
    return 0;
}
//*******
// END BMP BLOCK
//*******

int main() {
    std::cout << "Test Build" << std::endl;
    MHNGN_Pixel pixel = {100,200,300};
    MHNGN_Pixel * ppixel = new MHNGN_Pixel();
    byte* rgb;

    unsigned char* data = new unsigned char[512*512*3];
    ReadBMP(file_in, data);

    ImageMatrix imageData(WIDTH, ImageRowPixels(HEIGHT));
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH*3; j += 3) {
            imageData[i][j / 3] = {data[(i*WIDTH*3) + j] ,data[(i*WIDTH*3) + j + 1],data[(i*WIDTH*3) + j + 2]};
        }
    }
    imageData.shrink_to_fit();

    //*************
    // ACTUAL CODE
    // ************

    MHNG_math::sobelFilter(imageData);


    //************
    // END
    // ************

    unsigned char* processed_data = new unsigned char[512*512*3];
    int iter = 0;
    for (int i = 0; i < WIDTH; ++i) {
        for (int j = 0; j < HEIGHT; ++j) {
            processed_data[iter++] = (unsigned char)imageData[i][j].R;
            processed_data[iter++] = (unsigned char)imageData[i][j].G;
            processed_data[iter++] = (unsigned char)imageData[i][j].B;
        }
    }
    std::cout<<iter<<std::endl;
    delete []data;
    WriteBMP(file_out, processed_data);
    delete []processed_data;
    return 0;
}