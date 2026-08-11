#ifndef RASTERIZER_H
#define RASTERIZER_H

#include <stdint.h>
#include "obj.h"

//void set_pixel(int x,int y,float ooz,uint32_t color);
//void scanline_rasterize(vector* ndc_points,uint32_t face_color1,uint32_t face_color2);


void rasterize_barycentricFlat(VertexOutput* vertices,FlatShader* shader);
void rasterize_barycentricGouraud(VertexOutput* vertices,GouraudShader* shader);

#endif