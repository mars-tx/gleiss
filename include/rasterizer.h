#ifndef RASTERIZER_H
#define RASTERIZER_H

#include "shader.h"

void rasterize_barycentricFlat(VertexOutput* vertices,FlatShader* shader);
void rasterize_barycentricGouraud(VertexOutput* vertices,GouraudShader* shader);

void set_pixel(int x,int y,float ooz,uint32_t color);

#endif
