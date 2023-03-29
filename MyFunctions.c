#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include "MyFunctions.h"

unsigned char header[54] = { 0x42, 0x4d, 0, 0, 0, 0, 0, 0, 0, 0,54, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 1, 0, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0 };

int SSaveIMG(IMGObj* IObj, const char* fname) {
    FILE* fp_t = NULL;
    unsigned char* image_t = NULL;
    unsigned int file_size;
    int x, y, width = 2048, height = 2048, index;
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
    header[2] = (unsigned char)(file_size & 0x000000ff);
    header[3] = (file_size >> 8) & 0x000000ff;      header[4] = (file_size >> 16) & 0x000000ff;
    header[5] = (file_size >> 24) & 0x000000ff;
    header[18] = width & 0x000000ff;                header[19] = (width >> 8) & 0x000000ff;
    header[20] = (width >> 16) & 0x000000ff;        header[21] = (width >> 24) & 0x000000ff;
    header[22] = height & 0x000000ff;               header[23] = (width >> 8) & 0x000000ff;
    header[24] = (height >> 16) & 0x000000ff;       header[25] = (width >> 24) & 0x000000ff;
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
    int x, y, width = 2048, height = 2048, index;
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
    header[2] = (unsigned char)(file_size & 0x000000ff);
    header[3] = (file_size >> 8) & 0x000000ff;        header[4](file_size >> 16) & 0x000000ff;
    header[5] = (file_size >> 24) & 0x000000ff;
    header[18] = width & 0x000000ff;          header[19] = (width >> 8) & 0x000000ff;
    header[20] = (width >> 16) & 0x000000ff;  header[21] = (width >> 24) & 0x000000ff;
    header[22] = height & 0x000000ff;          header[23] = (width >> 8) & 0x000000ff;
    header[24] = (height >> 16) & 0x000000ff;  header[25] = (width >> 24) & 0x000000ff;
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