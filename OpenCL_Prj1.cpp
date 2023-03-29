#include <CL/opencl.h>
#include <iostream>
#include <chrono>
#include "MyFunctions.h"

int main(int argc, char* argv[]) {
    IMG_obj imgobj;

#pragma region CPU
    create_buffer(&imgobj);
    printf("1 read image\n\n");
    RReadIMG(&imgobj, "Ducks.BMP");
    printf("2 image_trans(CPU)\n");
    IMG_Trans(&imgobj);
    printf("3 save image\n\n");
    SSaveIMG(&imgobj, "Output_CPU.BMP");
#pragma endregion CPU

    printf("4 image_trans(GPU)\n");

#pragma region openCL
    cl_uint status;
    cl_platform_id platform;
    status = clGetPlatformIDs(1, &platform, NULL);

    cl_device_id device;
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU,
        1,
        &device,
        NULL);
    cl_context context = clCreateContext(NULL,
        1,
        &device,
        NULL, NULL, NULL);
    cl_command_queue queue = clCreateCommandQueue(context,
        device, CL_QUEUE_PROFILING_ENABLE, NULL);
    // ----------------------------------------------------------------------------------------
    cl_mem clbufR = clCreateBuffer(context,
        CL_MEM_READ_ONLY,
        IMG_Size * sizeof(cl_uchar), NULL,
        NULL);
    status = clEnqueueWriteBuffer(queue, clbufR, 1,
        0, IMG_Size * sizeof(cl_uchar), imgobj.sR, 0, 0, 0);
    // ----------------------------------------------------------------------------------------
    cl_mem clbufG = clCreateBuffer(context,
        CL_MEM_READ_ONLY,
        IMG_Size * sizeof(cl_uchar), NULL,
        NULL);
    status = clEnqueueWriteBuffer(queue, clbufG, 1,
        0, IMG_Size * sizeof(cl_uchar), imgobj.sG, 0, 0, 0);
    // ----------------------------------------------------------------------------------------
    cl_mem clbufB = clCreateBuffer(context,
        CL_MEM_READ_ONLY,
        IMG_Size * sizeof(cl_uchar), NULL,
        NULL);
    status = clEnqueueWriteBuffer(queue, clbufB, 1,
        0, IMG_Size * sizeof(cl_uchar), imgobj.sB, 0, 0, 0);
    // ----------------------------------------------------------------------------------------
    cl_mem clbufY = clCreateBuffer(context,
        CL_MEM_READ_ONLY,
        IMG_Size * sizeof(cl_uchar), NULL,
        NULL);
    status = clEnqueueWriteBuffer(queue, clbufY, 1,
        0, IMG_Size * sizeof(cl_uchar), imgobj.sY, 0, 0, 0);
    // ----------------------------------------------------------------------------------------
    const char* filename = "RGB2Y.cl";
    std::string sourceStr;
    status = convertToString(filename, sourceStr);
    const char* source = sourceStr.c_str();
    size_t sourceSize[] = { strlen(source) };

    // 創建程序對象
    cl_program program = clCreateProgramWithSource(
        context,
        1,
        &source,
        sourceSize,
        NULL);

    // 編譯程序對象
    status = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    if (status != 0) {
        printf("clBuide failed:%d\n", status);
        char tbuf[0x10000];
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0x10000, tbuf, NULL);
        printf("\n%s\n", tbuf);
        return -1;
    }

    // 創建kernel對象
    cl_kernel kernel = clCreateKernel(program, "RGB2Y", NULL);

    // 設置kernel參數
    cl_int clnum = IMG_Size;
    clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&clbufR);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&clbufG);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)&clbufB);
    clSetKernelArg(kernel, 3, sizeof(cl_mem), (void*)&clbufY);

    auto start = std::chrono::high_resolution_clock::now();
    // 執行kernel
    cl_event ev;
    size_t global_work_size = IMG_Size;
    clEnqueueNDRangeKernel(queue,
        kernel,
        1,
        NULL,
        &global_work_size,
        NULL, 0, NULL, &ev);

    clFinish(queue);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::ratio<1, 1000>> duration = end - start;
    std::cout << "GPU Time: " << duration.count() << " ms\n" << std::endl;

    // 數據Copy回host記憶體
    cl_uchar* ptr;
    ptr = (cl_uchar*)clEnqueueMapBuffer(queue,
        clbufY,
        CL_TRUE,
        CL_MAP_READ,
        0,
        IMG_Size * sizeof(cl_uchar),
        0, NULL, NULL, NULL);

    printf("5 save image\n\n");
    SSaveIMGX(ptr, "Output_GPU.BMP");

    clReleaseMemObject(clbufR);
    clReleaseMemObject(clbufG);
    clReleaseMemObject(clbufB);
    clReleaseMemObject(clbufY);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
#pragma endregion openCL

    printf("6 release buffer\n");
    delete_buffer(&imgobj);

    return 0;
}