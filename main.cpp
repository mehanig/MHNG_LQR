#include <iostream>
#include <array>
#include <fstream>
#include <string>
#include "typedefs.h"
#include "MHNG_math.h"
#include "file_operations.h"

std::string file_in = "./infile_1.bmp";
std::string file_out = "./infile_1_out-100.bmp";

int WIDTH;
int HEIGHT;
int FILESIZE;

int RESIZE = 0;

char* BMPHEADER = new char[54];

//*******
// READ BMP FOR TESTING ONLY 24 BITS
//*******

void getBMPSizes(std::string filename, int& WIDTH, int& HEIGHT, int& FILESIZE) {
    int i;
    FILE* f = fopen(filename.c_str(), "rb");

    if(f == NULL)
        throw "Argument Exception";

    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

    WIDTH = *(int*)&info[18];
    HEIGHT = *(int*)&info[22];
    int row_padded = (WIDTH*3 + 3) & (~3);
    FILESIZE = HEIGHT * row_padded;
    fclose(f);
}

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
    std::cout << "ROW " << row_padded << " ";
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
    fclose(f);
    return width * height * 3;
}


int WriteBMP(std::string filename, char* data) {
    std::ofstream f(filename, std::ios::out | std::ios::binary);
    f.write(BMPHEADER, 54);

    int width = *(int*)&BMPHEADER[18];
    int height = *(int*)&BMPHEADER[22];
    std::cout << std::endl;
    std::cout << "FILEsize" << FILESIZE << std::endl;
    std::cout << " Width: " << width << std::endl;
    std::cout << "Height: " << height << std::endl;
    f.write(data, FILESIZE);
    f.close();
    return 0;
}
//*******
// END BMP BLOCK
//*******

int main() {
    getBMPSizes(file_in, WIDTH, HEIGHT, FILESIZE);
    std::cout << WIDTH << " " << HEIGHT << " " << FILESIZE;
    unsigned char* data = new unsigned char[WIDTH*HEIGHT*3];
    ReadBMP(file_in, data);

    ImageMatrix imageData(WIDTH, ImageRowPixels(HEIGHT));
    std::cout << imageData.size() << "OK";
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH*3; j += 3) {
            imageData[j / 3][HEIGHT - i -1 ] = {data[i*WIDTH*3 + j] ,data[i*WIDTH*3 + j+1],data[i*WIDTH*3 + j+2]};
        }
    }
    imageData.shrink_to_fit();
    //*************
    // ACTUAL CODE
    // ************

    ImageMatrixGrayScale sobel = MHNG_math::sobelFilter(imageData);
    MHNG_SeamCarve carve = MHNG_math::getSeamCarveVertical(sobel);
//    ImageMatrixGrayScale resized = MHNG_math::resizeLQRHorisontal(sobel, RESIZE);

    ImageMatrix bw_image_sobel = MHNG_math::grayToRgb(sobel);


//    WIDTH = WIDTH - RESIZE;

    imageData = bw_image_sobel;
    for (auto k = carve.cbegin(); k != carve.cend(); ++k) {
        imageData[k->x][k->y].G = 0;
        imageData[k->x][k->y].B = 0;
        imageData[k->x][k->y].R = 250;
    }

    //************
    // END
    // ************
//
    int iter = 0;
    int row_padded = (WIDTH*3 + 3) & (~3);
//    BMPHEADER[18] = (char)WIDTH;
//    FILESIZE = HEIGHT * row_padded;
    char* processed_data = new char[FILESIZE];
    std::cout<<FILESIZE<<std::endl;
    for (int j = HEIGHT-1; j >= 0; j--) {
        int cur_in_row = 0;
        for (int i = 0; i < WIDTH - RESIZE; i++) {
            processed_data[iter++] = (unsigned char)imageData[i][j].R;
            processed_data[iter++] = (unsigned char)imageData[i][j].G;
            processed_data[iter++] = (unsigned char)imageData[i][j].B;
            cur_in_row +=3;
        }
        for (int i = 0; i < RESIZE; i++) {
            processed_data[iter++] = (unsigned char) 255;
            processed_data[iter++] = (unsigned char) 255;
            processed_data[iter++] = (unsigned char) 255;
            cur_in_row +=3;
        }
        while (cur_in_row < row_padded) {
            iter++;
            cur_in_row++;
        }
    }

    std::cout<<"WRITEN " <<iter<<std::endl;
    WriteBMP(file_out, processed_data);
    delete []data;
    delete []processed_data;
    delete []BMPHEADER;
    return 0;
}