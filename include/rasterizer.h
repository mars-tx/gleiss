#ifndef RASTERIZER_H
#define RASTERIZER_H

#include "shader.h"
#include "buffer.h"

void rasterize_barycentricFlat(Framebuffer*  buf,VertexOutput* vertices,FlatShader* shader);

void rasterize_barycentricGouraud(Framebuffer*  buf,VertexOutput* vertices,GouraudShader* shader);


void set_pixel(Framebuffer*  buf,int x,int y,float ooz,uint32_t color);

#endif
