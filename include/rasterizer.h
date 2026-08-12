#ifndef RASTERIZER_H
#define RASTERIZER_H

#include <stdint.h>
#include "obj.h"

typedef enum{SHADE_FLAT,SHADE_GOURAUD}shademode;


//void set_pixel(int x,int y,float ooz,uint32_t color);
//void scanline_rasterize(vector* ndc_points,uint32_t face_color1,uint32_t face_color2);

void barycentric(vec3* vertices,uint32_t basecolor,shademode mode);

bool bounding_box(vec3* vertices,int* xmin,int* xmax,int* ymin,int* ymax){
#endif