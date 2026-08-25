#ifndef RASTERIZER_H
#define RASTERIZER_H

#include "shader.h"
#include "buffer.h"

void rasterize_barycentricFlat(
        const VertexOutput* restrict vertices,
        FlatShader* restrict shader,
        Framebuffer* restrict buf);

void rasterize_barycentricGouraud(
        const VertexOutput* restrict vertices,
        GouraudShader* restrict shader,
        Framebuffer* restrict buf);


void set_pixel(
        Framebuffer* restrict buf,
        int x,int y,float ooz,
        uint32_t color);

#endif
