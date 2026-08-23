#include "../include/engine.h"
#include <stdio.h>
#include <stdlib.h>

EngineContext* engine_create(int width, int height, const char* title) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return NULL;
    }

    EngineContext* engine = malloc(sizeof(EngineContext));
    if (!engine) return NULL;

    engine->width = width;
    engine->height = height;
    engine->isRunning = true;

    engine->window = SDL_CreateWindow(title, width, height, 0);
    if (!engine->window){
        printf("Window Error: %s\n", SDL_GetError());
        destroy_Engine(engine);
        return NULL;
    }

    engine->renderer = SDL_CreateRenderer(engine->window, NULL);
    if (!engine->renderer){ 
        printf("Renderer Error: %s\n", SDL_GetError());
        destroy_Engine(engine);
        return NULL;}

    engine->texture = SDL_CreateTexture(
        engine->renderer, 
        SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_STREAMING, 
        width, height
    );
    if (!engine->texture){
        printf("Texture Error: %s\n", SDL_GetError());
        destroy_Engine(engine);
        return NULL;
    }

    engine->framebuffer = create_Framebuffer(width, height);
    if (!engine->framebuffer){
        destroy_Engine(engine);
        return NULL;
    }

    return engine;
}

void destroy_Engine(EngineContext* engine) {
    if (!engine) return;
    if (engine->framebuffer) free_Framebuffer(engine->framebuffer);
    if (engine->texture) SDL_DestroyTexture(engine->texture);
    if (engine->renderer) SDL_DestroyRenderer(engine->renderer);
    if (engine->window) SDL_DestroyWindow(engine->window);
    free(engine);
    SDL_Quit();
}

//Push pixels[] to SDL3 texture & display
void update_Engine(EngineContext* engine) {
    SDL_UpdateTexture(
        engine->texture, 
        NULL, 
        engine->framebuffer->pixels, 
        engine->width * sizeof(uint32_t)
    );
    SDL_RenderTexture(engine->renderer, engine->texture, NULL, NULL);
    SDL_RenderPresent(engine->renderer);
}
