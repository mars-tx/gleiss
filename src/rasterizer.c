#include <stdint.h>
#include <stdio.h>
#include "../include/utils.h"
#include "../include/vector.h"
#include "../include/shader.h"
#include "../include/buffer.h"

void rasterize_barycentricFlat(VertexOutput* vertices,FlatShader* shader,Framebuffer* restrict buf){

    int WIDTH= buf->width,
        HEIGHT= buf->height;

    //Clip space to screen space
    vec3 v1= vertices[0].clipPos;
    vec3 v2= vertices[1].clipPos;
    vec3 v3= vertices[2].clipPos;

    vec3 z_inv= {1.0f/v1.z,1.0f/v2.z,1.0f/v3.z};

    v1.x*= z_inv.x; v2.x*= z_inv.y; v3.x*= z_inv.z;
    v1.y*= z_inv.x; v2.y*= z_inv.y; v3.y*= z_inv.z;
    
    int x1= screen_x(v1.x,WIDTH),
        x2= screen_x(v2.x,WIDTH),
        x3= screen_x(v3.x,WIDTH);
    int y1= screen_y(v1.y,HEIGHT),
        y2= screen_y(v2.y,HEIGHT),
        y3= screen_y(v3.y,HEIGHT);

    int x13= x1 - x3,
        x23= x2 - x3;
    int y13= y1 - y3,
        y23= y2 - y3;
    float det= (x13*y23 - x23*y13);

    //Backface culling
    if(det>= 0){return;}

    det= 1.0f/det;
    float c1= y23*det, 
          c2= -y13*det;
    float r1= -x23*det, 
          r2= x13*det;

    //Color between 0-1
    Color vcolor1= vertices[0].vertexColor;
    Color vcolor2= vertices[1].vertexColor;
    Color vcolor3= vertices[2].vertexColor;

    uint32_t flatColor= fragment_FlatShader(shader,vcolor1,vcolor2,vcolor3);

    //Boundaries for rasterizer
    int xmax,xmin,ymax,ymin;
    xmin= MAX(MIN(x1,MIN(x2,x3)),0);
    xmax= MIN(WIDTH-1,MAX(x1,MAX(x2,x3)));
    
    ymin= MAX(MIN(y1,MIN(y2,y3)),0);
    ymax= MIN(HEIGHT-1,MAX(y1,MAX(y2,y3)));

    int xs3= xmin - x3, ys3= ymin - x3;

    float a= det*(y23*xs3 - x23*ys3);
    float b= det*(-y13*xs3 + x13*ys3);

    float u1,u2,u3,z_inv_pixel;
    int loc;
    for (int ys= ymin;ys<= ymax;ys++){
        for (int xs= xmin;xs<= xmax;xs++){

            u3= 1-u1-u2;
            if(u1>= 0 && u2>= 0 && u3>= 0){
                //Interpolating z
                z_inv_pixel= z_inv.x*u1 + z_inv.y*u2 + z_inv.z*u3;
                loc= ys*WIDTH + xs;
                if(z_inv_pixel> buf->z_buffer[loc]){
                    buf->z_buffer[loc]= z_inv_pixel;
                    buf->pixels[loc]= flatColor;
                }
            }
            u1+= c1;
            u2+= c2;
        }
    u1= a;
    u2= b;
    u1+= r1;
    u2+= r2;
    }
}

void rasterize_barycentricGouraud(VertexOutput* vertices,GouraudShader* shader,Framebuffer* restrict buf){

    int WIDTH= buf->width,
        HEIGHT= buf->height;

    //Clip space to screen space
    vec3 v1= vertices[0].clipPos;
    vec3 v2= vertices[1].clipPos;
    vec3 v3= vertices[2].clipPos;

    vec3 z_inv= {1.0f/v1.z,1.0f/v2.z,1.0f/v3.z};

    v1.x*= z_inv.x; v2.x*= z_inv.y; v3.x*= z_inv.z;
    v1.y*= z_inv.x; v2.y*= z_inv.y; v3.y*= z_inv.z;
    
    int x1= screen_x(v1.x,WIDTH),
        x2= screen_x(v2.x,WIDTH),
        x3= screen_x(v3.x,WIDTH);
    int y1= screen_y(v1.y,HEIGHT),
        y2= screen_y(v2.y,HEIGHT),
        y3= screen_y(v3.y,HEIGHT);

    int x13= x1 - x3,
        x23= x2 - x3;
    int y13= y1 - y3,
        y23= y2 - y3;
    float det= (x13*y23 - x23*y13);

    //Backface culling
    if(det>= 0){return;}

    det= 1.0f/det;
    float c1= y23*det, 
          c2= -y13*det;
    float r1= -x23*det, 
          r2= x13*det;

    //Setting values for interp
    shader->i1= vec3Dot(vertices[0].norm,shader->light_dir);
    shader->i2= vec3Dot(vertices[1].norm,shader->light_dir);
    shader->i3= vec3Dot(vertices[2].norm,shader->light_dir);

    //Color between 0-1
    Color vcolor1= vertices[0].vertexColor;
    Color vcolor2= vertices[1].vertexColor;
    Color vcolor3= vertices[2].vertexColor;

    //Boundaries for rasterizer
    int xmax,xmin,ymax,ymin;
    xmin= MAX(MIN(x1,MIN(x2,x3)),0);
    xmax= MIN(WIDTH-1,MAX(x1,MAX(x2,x3)));
    
    ymin= MAX(MIN(y1,MIN(y2,y3)),0);
    ymax= MIN(HEIGHT-1,MAX(y1,MAX(y2,y3)));

    int xs3= xmin - x3, ys3= ymin - x3;

    float a= det*(y23*xs3 - x23*ys3);
    float b= det*(-y13*xs3 + x13*ys3);

    float u1,u2,u3,z_inv_pixel;
    int loc;
    for (int ys= ymin;ys<= ymax;ys++){
        for (int xs= xmin;xs<= xmax;xs++){

            u3= 1-u1-u2;
            if(u1>= 0 && u2>= 0 && u3>= 0){
                //Interpolating z
                z_inv_pixel= z_inv.x*u1 + z_inv.y*u2 + z_inv.z*u3;
                loc= ys*WIDTH + xs;
                if(z_inv_pixel> buf->z_buffer[loc]){
                    buf->z_buffer[loc]= z_inv_pixel;

                    Color interp;
		            //maybe replace by diffs
                    interp.r= ((vcolor1.r - vcolor3.r)*u1 + (vcolor2.r - vcolor3.r)*u2 + vcolor3.r);
                    interp.g= ((vcolor1.g - vcolor3.g)*u1 + (vcolor2.g - vcolor3.g)*u2 + vcolor3.g);
                    interp.b= ((vcolor1.b - vcolor3.b)*u1 + (vcolor2.b - vcolor3.b)*u2 + vcolor3.b);
                    buf->pixels[loc]= fragment_GouraudShader(shader,interp,u1,u2);
                }
            }
            u1+= c1;
            u2+= c2;
        }
    u1= a;
    u2= b;
    u1+= r1;
    u2+= r2;
    }
}

void set_pixel(Framebuffer* restrict buf,int WIDTH,int x,int y,float ooz,uint32_t color){
        int loc= y*WIDTH + x;
        if (ooz> buf->z_buffer[loc]){
            buf->pixels[loc]= color;
            buf->z_buffer[loc]= ooz;
        }
}
