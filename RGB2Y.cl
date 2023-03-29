__kernel void RGB2Y(__global unsigned char* R,
                    __global unsigned char* G,
                    __global unsigned char* B,
                    __global unsigned char* Y)
{
    int id = get_global_id(0);
    float pY = 0.299 * (float)R[id] + 0.587 * (float)G[id] + 0.144 * (float)B[id];
    pY = (int)pY + 0.5;
    pY = ((pY > 250) ? 255 : ((pY < 5) ? 0 : pY));
    Y[id] = (unsigned char)pY;
}

