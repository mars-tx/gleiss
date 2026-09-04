#include <stdio.h>
#include <stdint.h>
#include "../include/utils.h"
#include "../include/vector.h"
#include "../include/shader.h"
#include "../include/buffer.h"

void rasterize_barycentricFlat(
        const VertexOutput* vertices,
        FlatShader* shader,
        Framebuffer* buf){

    int WIDTH= buf->width,
        HEIGHT= buf->height;

    //Clip space to screen space
    vec3 v1= vertices[0].clip_pos;
    vec3 v2= vertices[1].clip_pos;
    vec3 v3= vertices[2].clip_pos;

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
    //if(det>= 0){return;}

    det= 1.0f/det;
    float c1= y23*det, 
          c2= -y13*det;
    float r1= -x23*det, 
          r2= x13*det;

    //Color between 0-255

    //uint32_t flatColor= 0xf0f000ff;
    float intensity= vec3Dot(shader->face_norm,shader->light_dir);
    uint32_t flatColor= fragment_FlatShader(
            intensity,vertices[0].vertex_color,
            vertices[1].vertex_color,vertices[2].vertex_color);

    //Boundaries for rasterizer
    int xmax,xmin,ymax,ymin;
    xmin= mini(x1,mini(x2,x3));
    xmax= maxi(x1,maxi(x2,x3));
    xmin= maxi(0,xmin);
    xmax= mini(xmax,WIDTH-1);
    
    ymin= mini(y1,mini(y2,y3));
    ymax= maxi(y1,maxi(y2,y3));
    ymin= maxi(0,ymin);
    ymax= mini(ymax,HEIGHT-1);
    
    int xs3= xmin - x3, ys3= ymin - y3;

    float a= det*(y23*xs3 - x23*ys3);
    float b= det*(-y13*xs3 + x13*ys3);

    float u1 = a,u2 = b,z_inv_pixel;
    int loc;
    for (int ys= ymin;ys<= ymax;ys++){
        for (int xs= xmin;xs<= xmax;xs++){

            if(u1>= 0 && u2>= 0 && (u1 + u2)<= 1){
                //Interpolating z
                z_inv_pixel= (z_inv.x - z_inv.z)*u1 
                           + (z_inv.y - z_inv.z)*u2 + z_inv.z;

                loc= ys*WIDTH + xs;
                if(z_inv_pixel > buf->z_buffer[loc]){
                    buf->z_buffer[loc]= z_inv_pixel;
                    buf->pixels[loc]= flatColor;
                }
            }
            u1+= c1;
            u2+= c2;
        }
        u1= a;
        u2= b;
        a+= r1;
        b+= r2;
    }
}

void rasterize_barycentricGouraud(
        const VertexOutput* vertices,
        GouraudShader* shader,
        Framebuffer* buf){

    int WIDTH= buf->width,HEIGHT= buf->height;

    //Clip space to screen space
    vec3 v1= vertices[0].clip_pos;
    vec3 v2= vertices[1].clip_pos;
    vec3 v3= vertices[2].clip_pos;

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
    //if(det>= 0){return;}

    det= 1.0f/det;
    float c1= y23*det, 
          c2= -y13*det;
    float r1= -x23*det, 
          r2= x13*det;

    //Setting values for interp
    float i1= vec3Dot(vertices[0].norm,shader->light_dir);
    float i2= vec3Dot(vertices[1].norm,shader->light_dir);
    float i3= vec3Dot(vertices[2].norm,shader->light_dir);

    //Color between 0-255
    uint32_t vcolor1= vertices[0].vertex_color;
    uint32_t vcolor2= vertices[1].vertex_color;

    uint32_t vcolor3_r= vertices[2].vertex_color >> 24 & 0xFF;
    uint32_t vcolor3_g= vertices[2].vertex_color >> 16 & 0xFF;
    uint32_t vcolor3_b= vertices[2].vertex_color >> 8 & 0xFF;
    uint32_t vcolor3_a= vertices[2].vertex_color & 0xFF;

    //Boundaries for rasterizer
    int xmax,xmin,ymax,ymin;
    xmin= mini(x1,mini(x2,x3));
    xmax= maxi(x1,maxi(x2,x3));
    xmin= maxi(0,xmin);
    xmax= mini(xmax,WIDTH-1);
    
    ymin= mini(y1,mini(y2,y3));
    ymax= maxi(y1,maxi(y2,y3));
    ymin= maxi(0,ymin);
    ymax= mini(ymax,HEIGHT-1);
    
    int xs3= xmin - x3, ys3= ymin - y3;

    float a= det*(y23*xs3 - x23*ys3);
    float b= det*(-y13*xs3 + x13*ys3);

    float u1 = a,u2 = b,z_inv_pixel,diffuse;
    int loc;
    for (int ys= ymin;ys<= ymax;ys++){
        for (int xs= xmin;xs<= xmax;xs++){

            if(u1>= 0 && u2>= 0 && (u1 + u2)<= 1){
                //Interpolating z
                z_inv_pixel= (z_inv.x - z_inv.z)*u1 
                           + (z_inv.y - z_inv.z)*u2 + z_inv.z;

                loc= ys*WIDTH + xs;
                if(z_inv_pixel > buf->z_buffer[loc]){

                    diffuse= u1* (i1 - i3) + u2* (i2 - i3) + i3;
                    Color interp;
                    interp.r= ((vcolor1 >> 24 & 0xFF) - vcolor3_r)*u1 
                            + ((vcolor2 >> 24 & 0xFF) - vcolor3_r)*u2 
                            + vcolor3_r;
                    interp.g= ((vcolor1 >> 16 & 0xFF) - vcolor3_g)*u1 
                            + ((vcolor2 >> 16 & 0xFF) - vcolor3_g)*u2 
                            + vcolor3_g;
                    interp.b= ((vcolor1 >> 8 & 0xFF) - vcolor3_b)*u1 
                            + ((vcolor2 >> 8 & 0xFF) - vcolor3_b)*u2 
                            + vcolor3_b;
                    interp.a= ((vcolor1 & 0xFF) - vcolor3_a)*u1 
                            + ((vcolor2 & 0xFF) - vcolor3_a)*u2 
                            + vcolor3_a;

                    buf->z_buffer[loc]= z_inv_pixel;
                    buf->pixels[loc]= 0x00f0f0ff;
                    //fragment_GouraudShader(interp,diffuse);
                }
            }
            u1+= c1;
            u2+= c2;
        }
        u1= a;
        u2= b;
        a+= r1;
        b+= r2;
    }
}

void set_pixel(
        Framebuffer* buf,
        int x,int y,float ooz,
        uint32_t color){

        int loc= y*buf->width + x;
        if (ooz> buf->z_buffer[loc]){
            buf->pixels[loc]= color;
            buf->z_buffer[loc]= ooz;
        }
}
