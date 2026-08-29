#ifndef RASTERIZER_H
#define RASTERIZER_H

#include "shader.h"
#include "buffer.h"

void rasterize_barycentricFlat(
        const VertexOutput* vertices,
        FlatShader* shader,
        Framebuffer* buf);

void rasterize_barycentricGouraud(
        const VertexOutput* vertices,
        GouraudShader* shader,
        Framebuffer* buf);


void set_pixel(
        Framebuffer* buf,
        int x,int y,float ooz,
        uint32_t color);

#endif
