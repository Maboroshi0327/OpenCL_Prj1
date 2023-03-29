#ifndef __MYFUNCTIONS
#define __MYFUNCTIONS

#ifdef __cplusplus
extern "C" {
#endif

#define IMG_W 2048
#define IMG_H 2048
#define IMG_Size 4194304

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
int IMG_Trans(IMGObj* IObj);

#ifdef __cplusplus
}
#endif

#endif  /* __MYFUNCTIONS   */