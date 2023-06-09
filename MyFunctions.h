#ifndef __MYFUNCTIONS
#define __MYFUNCTIONS

#define IMG_W 1920
#define IMG_H 1080
#define IMG_Size IMG_W * IMG_H

typedef struct IMG_obj {
    unsigned char* InputSrc, * InputData;
    unsigned char* sR, * sG, * sB, * sY;
    unsigned short IMGW, IMGH;
} IMGObj, * pImgObj;

int RReadIMG(IMGObj* IObj, const char* fname);
int SSaveIMG(IMGObj* IObj, const char* fname);
int SSaveIMGX(unsigned char* ptr, const char* fname);
int create_buffer(IMGObj* IObj);
int delete_buffer(IMGObj* IObj);
int IMG_Trans(IMGObj* IObj, double& timeCPU);
int convertToString(const char* filename, std::string& s);

#endif  /* __MYFUNCTIONS   */