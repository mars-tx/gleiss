#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>

#define WIDTH 800
#define HEIGHT 600

#define Z_NEAR 0.1f
#define Z_FAR 100.0f

extern uint32_t pixels[WIDTH*HEIGHT];
extern float z_buffer[WIDTH*HEIGHT];

#define screen_x(x)  ((int)((x+1.0f)*0.5f*WIDTH))
#define screen_y(y)  ((int)((-y+1.0f)*0.5f*HEIGHT))
#define MAX(x,y)    (((x)>= (y))? (x) : (y))
#define MIN(x,y)    (((x)<= (y))? (x) : (y))

void create_window();
void init_renderer();
void init_texture();

#endif
