#include <CL/opencl.h>
#include <iostream>
#include "MyFunctions.h"

typedef struct IMG_obj {
    unsigned char* InputSrc, * InputData;
    unsigned char* sR, * sG, * sB, * sY;
    unsigned short IMGW, IMGH;
} IMGObj, * pImgObj;
IMGObj imgobj;

//----------------------------------------------------------------------------------------------------------------



