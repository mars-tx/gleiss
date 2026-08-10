#include <stdio.h>
#include "SDL3/SDL.h"
#include "../include/main.h"

void create_window(){
    window= SDL_CreateWindow("hello-window",WIDTH,HEIGHT,0);
    if (window== NULL) {
        printf("Window_Error: %s\n", SDL_GetError());
        SDL_Quit();
    }
    return;
}

void init_renderer(){
    renderer= SDL_CreateRenderer(window, NULL);
    if (renderer== NULL) {
        printf("Renderer_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    return;
}

void init_texture(){
    texture= SDL_CreateTexture(renderer,pixel_format,SDL_TEXTUREACCESS_STREAMING,WIDTH,HEIGHT);
    if(texture== NULL){
        printf("texture_error %s",SDL_GetError());
        SDL_DestroyWindow(window); 
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
    }
    return;
}