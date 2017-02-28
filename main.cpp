#include <iostream>
#include <array>
#include <fstream>
#include <string>
#include "typedefs.h"
#include "MHNG_math.h"
#include "file_operations.h"

std::string file_in = "./mhng.bmp";
std::string file_out = "./loloutfile_next.bmp";

int WIDTH = 1222;
int HEIGHT = 984;
int rgb_size = HEIGHT * WIDTH * 3;


char* BMPHEADER = new char[54];
int FILESIZE = 0;

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
//    std::cout << " WAS " << (int)data[783477] << std::endl;
    fclose(f);
    return width * height * 3;
}


int WriteBMP(std::string filename, char* data) {
    std::ofstream f(filename, std::ios::out | std::ios::binary);
    f.write(BMPHEADER, 54);
//    f.close();

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
    std::cout << "Test Build" << std::endl;

    unsigned char* data = new unsigned char[WIDTH*HEIGHT*3];
    ReadBMP(file_in, data);

    ImageMatrix imageData(WIDTH, ImageRowPixels(HEIGHT));
    std::cout << imageData.size() << "OK";
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH*3; j += 3) {
            imageData[j / 3][HEIGHT - i] = {data[i*WIDTH*3 + j] ,data[i*WIDTH*3 + j+1],data[i*WIDTH*3 + j+2]};
//            if (imageData[i][j / 3].B != 0 || imageData[i][j / 3].G != 0 || imageData[i][j / 3].R != 0) {
//                std::cout<<"\ni,j:"<<i<<" "<<j / 3 << " "<<imageData[i][j /3 ].B<<" "<<imageData[i][j / 3].G<<" "<<imageData[i][j / 3].R<<"|\n";
//            }
//            std::cout<<"\nR:"<<imageData[i][j].R<<"\n";
        }
    }
    int tmp;
    for (int i = 0; i < WIDTH; ++i) {
        for (int j = 0; j < HEIGHT; ++j) {
//            if (imageData[i][j].B != 0 || imageData[i][j].G != 0 || imageData[i][j].R != 0) {
//                std::cout<<"\ni,j:"<<i<<" "<<j<< " "<<imageData[i][j].B<<" "<<imageData[i][j].G<<" "<<imageData[i][j].R<<"|\n";
//            }
        }
    }
    imageData.shrink_to_fit();
    //*************
    // ACTUAL CODE
    // ************

    ImageMatrixGrayScale sobel = MHNG_math::sobelFilter(imageData);
    ImageMatrix bw_image_sobel = MHNG_math::grayToRgb(sobel);
    imageData = bw_image_sobel;

    //************
    // END
    // ************
//
    std::cout<<"AAAA"<<std::endl;
    int iter = 0;
    int row_padded = (WIDTH*3 + 3) & (~3);
    FILESIZE = HEIGHT * row_padded;
    char* processed_data = new char[FILESIZE];
    for (int j = HEIGHT-1; j >= 0; j--) {
        int cur_in_row = 0;
        for (int i = 0; i < WIDTH; ++i) {
//            if (imageData[i][j].B != 0 || imageData[i][j].G != 0 || imageData[i][j].R != 0) {
//                std::cout<<"\ni,j:"<<i<<" "<<j<< " "<<imageData[i][j].B<<" "<<imageData[i][j].G<<" "<<imageData[i][j].R<<"|\n";
//            }
//            int r = imageData[i][j].R;
//            char aaa = (char)
            processed_data[iter++] = (unsigned char)imageData[i][j].R;
            processed_data[iter++] = (unsigned char)imageData[i][j].G;
            processed_data[iter++] = (unsigned char)imageData[i][j].B;
            cur_in_row +=3;
        }
//        while (cur_in_row < row_padded) {
//            iter++;
//            cur_in_row++;
//        }
    }

    std::cout<<"WRITEN " <<iter<<std::endl;
//    std::cout<<"DATA:"<<processed_data<<std::endl;
    WriteBMP(file_out, processed_data);
    delete []data;
    delete []processed_data;
    delete []BMPHEADER;
    return 0;
}