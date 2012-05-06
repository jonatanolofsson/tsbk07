__constant sampler_t smp = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;
__constant int k[9] = {1, 2, 1, 2, 4, 2, 1, 2, 1};

__kernel void blurr(__read_only image2d_t src_img, __write_only image2d_t dst_img)
{
    int2 coord = (int2)(get_global_id(0), get_global_id(1));

    float4 color;
    int index = 0;
    for(int dx = -1; dx <= 1; ++dx) {
        for(int dy = -1; dy <= 1; ++dy) {
            color += read_imagef(src_img, smp, (int2)(coord.x + dx, coord.y + dy));
        }  
    }
    color /= 9.0;
    
    barrier(CLK_GLOBAL_MEM_FENCE);
    write_imagef(dst_img, coord, color);
}
