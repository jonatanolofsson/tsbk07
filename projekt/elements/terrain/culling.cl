__kernel void generate_indices(__global unsigned int* indices, unsigned int N, unsigned int bottom, unsigned int x, unsigned int Z_stride)
{
    unsigned int n = get_local_id(0);
    unsigned int z = n + bottom;
    __global unsigned int* ind = indices + 6 * (N + n);
    unsigned int q = Z_stride * x + z;
    unsigned int w = Z_stride;
                ind[0]  =               q; // 3 * (Z_stride * x + z); //index<Z,3>(zi,xi);
                ind[1]  =   ind[4]  =   q + w; // 3 * (Z_stride * (x + 1) + z); //index<Z,3>(zi,xi+1);
                ind[2]  =   ind[3]  =   q + 1; // 3 * (Z_stride * x + z + 1); //index<Z,3>(zi+1,xi);
                            ind[5]  =   q + w + 1; // 3 * (Z_stride * (x + 1) + z + 1); //index<Z,3>(zi+1,xi+1);
}
