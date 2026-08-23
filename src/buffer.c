#include <stdint.h>
#include <stdlib.h>
#include "../include/buffer.h"

Framebuffer* create_Framebuffer(int width, int height) {
    Framebuffer* fb = malloc(sizeof(Framebuffer));

    if (!fb) return NULL;

    fb->width = width;
    fb->height = height;
    fb->pixels = malloc(sizeof(uint32_t) * width * height);
    fb->z_buffer = malloc(sizeof(float) * width * height);

    if (!fb->pixels || !fb->z_buffer) {
        free_Framebuffer(fb);
        return NULL;
    }
    return fb;
}

void clear_Framebuffer(Framebuffer* restrict fb, uint32_t color) {
    int total = fb->width * fb->height;
    for (int i = 0; i < total; i++) {
        fb->pixels[i] = color;
        fb->z_buffer[i] = 0.0f;
    }
}

void free_Framebuffer(Framebuffer* restrict fb) {
    if (fb) {
        if (fb->pixels) free(fb->pixels);
        if (fb->z_buffer) free(fb->z_buffer);
        free(fb);
    }
}

