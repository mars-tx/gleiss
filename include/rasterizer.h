#ifndef RASTERIZER_H
#define RASTERIZER_H

#include "shader.h"
#include "buffer.h"

void rasterize_barycentricFlat(Framebuffer* restrict buf,VertexOutput* vertices,FlatShader* shader);

void rasterize_barycentricGouraud(Framebuffer* restrict buf,VertexOutput* vertices,GouraudShader* shader);


void set_pixel(Framebuffer* restrict buf,int x,int y,float ooz,uint32_t color);

#endif
