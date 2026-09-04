#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../include/buffer.h"

Framebuffer* create_Framebuffer(int width, int height){
    Framebuffer* fb= malloc(sizeof(Framebuffer));

    if (!fb) return NULL;

    fb->width= width;
    fb->height= height;
    fb->pixels= malloc(sizeof(uint32_t) * width * height);
    fb->z_buffer= malloc(sizeof(float) * width * height);

    if (!fb->pixels || !fb->z_buffer) {
        free_Framebuffer(fb);
        return NULL;
    }
    return fb;
}

void clear_Framebuffer(Framebuffer* restrict fb, uint32_t color){
    int total = fb->width * fb->height;
    
    memset(fb->z_buffer, 0, total * sizeof(float));

    uint64_t color64 = ((uint64_t)color << 32) | color;
    uint64_t* pixels64 = (uint64_t*)fb->pixels;
    int total64 = total / 2;

    //Write 2 pixels per cycle
    for (int i = 0; i < total64; i++) {
        pixels64[i] = color64;
    }
    
    if (total % 2 != 0) {
        fb->pixels[total - 1] = color;
    }
}

void free_Framebuffer(Framebuffer* fb){
    if (fb){
        if (fb->pixels) free(fb->pixels);
        if (fb->z_buffer) free(fb->z_buffer);
        free(fb);
    }
}

