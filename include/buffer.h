#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>

static inline int screen_x(float x, int width) {
    return (int)((x + 1.0f) * 0.5f * width);
}

static inline int screen_y(float y, int height) {
    return (int)((-y + 1.0f) * 0.5f * height);
}

typedef struct{
    uint32_t* pixels;
    float* z_buffer;
    int width;
    int height;

}Framebuffer;

Framebuffer* create_Framebuffer(int width, int height);

void free_Framebuffer(Framebuffer* restrict fb);

void clear_Framebuffer(Framebuffer* restrict fb, uint32_t color);

#endif
