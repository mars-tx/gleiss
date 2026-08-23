#ifndef ENGINE_H
#define ENGINE_H

#include <stdbool.h>
#include <SDL3/SDL.h>
#include "obj.h"
#include "buffer.h"

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    Framebuffer* framebuffer;
    int width;
    int height;
    bool isRunning;
} EngineContext;

EngineContext* create_Engine(int width, int height, const char* title);

void destroy_Engine(EngineContext* engine);

void update_Engine(EngineContext* engine);

#endif
