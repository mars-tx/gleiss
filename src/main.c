#include "SDL3/SDL.h" 
#include "math.h"
#include "unistd.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "include/obj.h"
#include "include/main.h"
#include "include/matrix.h"
#include "include/rasterizer.h"

SDL_Window* window= NULL;
SDL_Renderer* renderer= NULL;
SDL_Texture* texture= NULL;
SDL_PixelFormat pixel_format= SDL_PIXELFORMAT_RGBA8888;

mat4 PV;

uint32_t pixels[WIDTH*HEIGHT];
float z_buffer[WIDTH*HEIGHT];
vec3 lightDir= {0.0f,0.0f,1.0f};

void clean_buffer();

int main(){

    create_window();
    init_renderer();
    init_texture();

    memset(z_buffer,0,sizeof(z_buffer));
    clean_buffer();

    bool isRunning= true;
    SDL_Event event;

    while (isRunning) {
        while (SDL_PollEvent(&event)){
            if ((&event)->type== SDL_EVENT_QUIT) {
                isRunning= false;
            }
        }
        clean_buffer();
        memset(z_buffer,0,sizeof(z_buffer));

        /*cam->target.y+=0.1f;
        create_view_lookat_mat(cam);
        projview= mat_mult_mat(proj,&cam->view);*/

        //CPU to PCIe lanes to VRAM 
        SDL_UpdateTexture(texture,NULL,pixels,WIDTH*4);         

        //Draws into buffer
        SDL_RenderTexture(renderer,texture,NULL,NULL);          
        
        //Swap GPU buffers
        SDL_RenderPresent(renderer);            

        sleep(2);break;
        
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


void clean_buffer(){
    uint32_t bg= 0x000000ff;
    for(int i=0;i<WIDTH*HEIGHT;i++){
        pixels[i]= bg;
    }
}

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