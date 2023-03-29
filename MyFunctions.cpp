#define _CRT_SECURE_NO_WARNINGS
//#include <cstdio>
#include <iostream>
#include <chrono>
#include <fstream>
#include "MyFunctions.h"

//                           ---type---    ---size---     -reserved1-   -reserved2-    ---offset---    ----size----
unsigned char header[54] = { 0x42, 0x4d,   0, 0, 0, 0,      0, 0,         0, 0,       54, 0, 0, 0,    40, 0, 0, 0,

//                           ---width---   --height--     --planes--    ---bits---    -compression-   --imagesize--
                             0, 0, 0, 0,   0, 0, 0, 0,      1, 0,        24, 0,       0, 0, 0, 0,     0, 0, 0, 0,

//                          -xresolution-  -yresolution-  --ncolours--  --importantcolours--
                             0, 0, 0, 0,    0, 0, 0, 0,    0, 0, 0, 0,      0, 0, 0, 0
};


int RReadIMG(IMGObj* IObj, const char* fname) {
    FILE* fp_s = NULL;
    unsigned int x, y, width, height, offset;
    unsigned char* image_s = NULL;
    unsigned short file_size, rgb_raw_data_offset, bit_per_pixel, byte_per_pixel;
    fp_s = fopen(fname, "rb");
    if (fp_s == NULL) {
        printf("fopen fp_s error @ read image\n");
        return -1;
    }
    fseek(fp_s, 10, SEEK_SET);          fread(&rgb_raw_data_offset, sizeof(unsigned short), 1, fp_s);
    fseek(fp_s, 18, SEEK_SET);          fread(&width, sizeof(unsigned int), 1, fp_s);
    fread(&height, sizeof(unsigned int), 1, fp_s);
    fseek(fp_s, 28, SEEK_SET);          fread(&bit_per_pixel, sizeof(unsigned short), 1, fp_s);
    byte_per_pixel = bit_per_pixel / 8;
    fseek(fp_s, 54, SEEK_SET);
    fread(IObj->InputSrc, sizeof(unsigned char), (size_t)(long)width * height * byte_per_pixel, fp_s);
    if (IObj->InputSrc == NULL) {
        printf("malloc images_s error [RReadIMG]\n");
        fclose(fp_s);
        return -1;
    }
    fclose(fp_s);
    return 0;
}

int SSaveIMG(IMGObj* IObj, const char* fname) {
    FILE* fp_t = NULL;
    unsigned char* image_t = NULL;
    unsigned int file_size;
    int x, y, width = IMG_W, height = IMG_H, index;
    image_t = (unsigned char*)malloc((size_t)(width)*height * 3);
    if (image_t == NULL) { printf("saveIMG malloc image_t error\n"); return -1; }
    index = 0;
    for (y = 0; y != height; y++) {
        for (x = 0; x != width; x++) {
            *(image_t + 3 * index + 2) = (unsigned char)IObj->sY[index];
            *(image_t + 3 * index + 1) = (unsigned char)IObj->sY[index];
            *(image_t + 3 * index + 0) = (unsigned char)IObj->sY[index];
            index++;
        }
    }
    fp_t = fopen(fname, "wb");
    if (fp_t == NULL) { printf("saveIMG fopen fname error\n"); return -1; }
    file_size = width * height * 3 + 54;

    // file_size
    header[2] = (unsigned char)(file_size & 0x000000ff);
    header[3] = (file_size >> 8) & 0x000000ff;
    header[4] = (file_size >> 16) & 0x000000ff;
    header[5] = (file_size >> 24) & 0x000000ff;

    // width
    header[18] = width & 0x000000ff;
    header[19] = (width >> 8) & 0x000000ff;
    header[20] = (width >> 16) & 0x000000ff;
    header[21] = (width >> 24) & 0x000000ff;

    // height
    header[22] = height & 0x000000ff;
    header[23] = (height >> 8) & 0x000000ff;
    header[24] = (height >> 16) & 0x000000ff;
    header[25] = (height >> 24) & 0x000000ff;

    // bits
    header[28] = 24;

    fwrite(header, sizeof(unsigned char), 54, fp_t);
    fwrite(image_t, sizeof(unsigned char), (size_t)(long)width * height * 3, fp_t);
    fclose(fp_t);
    free(image_t);
    return 0;
}

int SSaveIMGX(unsigned char* ptr, const char* fname) {
    FILE* fp_t = NULL;
    unsigned char* image_t = NULL;
    unsigned int file_size;
    int x, y, width = IMG_W, height = IMG_H, index;
    image_t = (unsigned char*)malloc((size_t)(width)*height * 3);
    if (image_t == NULL) { printf("saveIMG malloc image_t error\n"); return -1; }
    index = 0;
    for (y = 0; y != height; y++) {
        for (x = 0; x != width; x++) {
            *(image_t + 3 * index + 2) = (unsigned char)ptr[index];
            *(image_t + 3 * index + 1) = (unsigned char)ptr[index];
            *(image_t + 3 * index + 0) = (unsigned char)ptr[index];
            index++;
        }
    }
    fp_t = fopen(fname, "wb");
    if (fp_t == NULL) { printf("saveIMG fopen fname error\n"); return -1; }
    file_size = width * height * 3 + 54;

    // file_size
    header[2] = (unsigned char)(file_size & 0x000000ff);
    header[3] = (file_size >> 8) & 0x000000ff;
    header[4] = (file_size >> 16) & 0x000000ff;
    header[5] = (file_size >> 24) & 0x000000ff;

    // width
    header[18] = width & 0x000000ff;                
    header[19] = (width >> 8) & 0x000000ff;
    header[20] = (width >> 16) & 0x000000ff;        
    header[21] = (width >> 24) & 0x000000ff;

    // height
    header[22] = height & 0x000000ff;               
    header[23] = (height >> 8) & 0x000000ff;
    header[24] = (height >> 16) & 0x000000ff;
    header[25] = (height >> 24) & 0x000000ff;

    // bits
    header[28] = 24;

    fwrite(header, sizeof(unsigned char), 54, fp_t);
    fwrite(image_t, sizeof(unsigned char), (size_t)(long)width * height * 3, fp_t);
    fclose(fp_t);
    free(image_t);
    return 0;
}

int create_buffer(IMGObj* IObj) {
    IObj->IMGW = IMG_H;
    IObj->IMGH = IMG_W;
    IObj->InputSrc = (unsigned char*)malloc(sizeof(unsigned char) * IMG_Size * 3 + 54);
    IObj->InputData = IObj->InputSrc + 54;

    IObj->sR = (unsigned char*)malloc(sizeof(unsigned char) * IMG_Size);
    IObj->sG = (unsigned char*)malloc(sizeof(unsigned char) * IMG_Size);
    IObj->sB = (unsigned char*)malloc(sizeof(unsigned char) * IMG_Size);
    IObj->sY = (unsigned char*)malloc(sizeof(unsigned char) * IMG_Size);
    return 0;
}

int delete_buffer(IMGObj* IObj) {
    free(IObj->InputSrc);
    free(IObj->sR); free(IObj->sG); free(IObj->sB); free(IObj->sY);
    return 0;
}

int IMG_Trans(IMGObj* IObj, double& timeCPU) {
    int x, y, index = 0, off = 0;
    for (y = 0; y != IMG_H; y++) {
        for (x = 0; x != IMG_W; x++)
        {
            IObj->sR[off] = IObj->InputSrc[index + 2];
            IObj->sG[off] = IObj->InputSrc[index + 1];
            IObj->sB[off] = IObj->InputSrc[index + 0];
            off++; index += 3;
        }
    }

    float pixel_Y;
    index = 0;

    auto start = std::chrono::high_resolution_clock::now();
    for (y = 0; y != IMG_H; y++) {
        for (x = 0; x != IMG_W; x++)
        {
            pixel_Y = 0.299 * (float)IObj->sR[index] + 0.587 * (float)IObj->sG[index] + 0.114 * (float)IObj->sB[index];
            pixel_Y = (int)(pixel_Y + 0.5);
            pixel_Y = ((pixel_Y > 250) ? 255 : ((pixel_Y < 5) ? 0 : pixel_Y));
            IObj->sY[index] = (unsigned)pixel_Y;
            index++;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::ratio<1, 1000>> duration = end - start;
    std::cout << "CPU Time: " << duration.count() << " ms\n" << std::endl;
    timeCPU = duration.count();

    return 0;
}

int convertToString(const char* filename, std::string& s)
{
    size_t size;
    char* str;
    std::fstream f(filename, (std::fstream::in | std::fstream::binary));

    if (f.is_open())
    {
        size_t fileSize;
        f.seekg(0, std::fstream::end);
        size = fileSize = (size_t)f.tellg();
        f.seekg(0, std::fstream::beg);

        str = new char[size + 1];
        if (!str)
        {
            f.close();
            return NULL;
        }

        f.read(str, fileSize);
        f.close();
        str[size] = '\0';

        s = str;
        delete[] str;
        return 0;
    }
    printf("Error: Failed to open file %s\n", filename);
    return 1;
}