#include <stdio.h>
#include "math.h"
#include "include/obj.h"
#include "include/main.h"
#include "include/shader.h"

void draw_triangle_barycentric(VertexOutput* vertices,void* shader,uint32_t basecolor,Shademode mode){

    if (mode== SHADE_FLAT){shader= (FlatShader*)shader;}
    else if (mode== SHADE_GOURAUD){shader= (GouraudShader*)shader;}

    vec3 v1= vertices[0].clip_pos;
    vec3 v2= vertices[1].clip_pos;
    vec3 v3= vertices[2].clip_pos;

    vec3 z_inv= {1.0f/v1.z,1.0f/v2.z,1.0f/v3.z};

    //Clip space to screen space
    v1.x*= z_inv.x; v2.x*= z_inv.y; v3.x*= z_inv.z;
    v1.y*= z_inv.x; v2.y*= z_inv.y; v3.y*= z_inv.z;
    
    int x1= screen_x(v1.x),x2= screen_x(v2.x),x3= screen_x(v3.x);
    int y1= screen_y(v1.y),y2= screen_y(v2.y),y3= screen_y(v3.y);

    int x13= x1 - x3,x23= x2 - x3;
    int y13= y1 - y3,y23= y2 - y3;
    float det= (x13* y23 - x23* y13);
    //if(det>= 0){return;}
    det= 1.0f/det;
    float c1= y23*det, c2= -y13*det;
    float r1= -x23*det, r2= x13*det;

    //Boundaries for rasterizer
    int xmax,xmin,ymax,ymin;
    bounding_box(x1,y1,x2,y2,x3,y3,&xmin,&xmax,&ymin,&ymax);
    int xs3= xmin - x3, ys3= ymin - x3;

    float u1,u2,u3,z_inv_pixel;
    if (mode == SHADE_FLAT){
        //basecolor= (basecolor & 0xffffff00) | (int)((basecolor & 0x000000ff)* i);

        float a= det*(y23*xs3 - x23*ys3);
        float b= det*(-y13*xs3 + x13*ys3);
        int loc;
        for (int ys= ymin;ys<= ymax;ys++){
            for (int xs= xmin;xs<= xmax;xs++){

                u3= 1-u1-u2;
                if(u1>= 0 && u2>= 0 && u3>= 0){
                    //Interpolating z
                    z_inv_pixel= z_inv.x* u1+ z_inv.y* u2+ z_inv.z* u3;
                    loc= ys* WIDTH+ xs;
                    if(z_inv_pixel> z_buffer[loc]){
                        z_buffer[loc]= z_inv_pixel;
                        //fragment_Shader(shader,u1,u2);
                        pixels[loc]= basecolor;
                    }
                }
                u1+= c1;
                u2+= c2;
            }
        u1= a;
        u2= b;
        u1+= r1;
        u2+= r2;
        r1+= r1;r2+= r2;
        }
    }
}

void draw_bounding_box(int x1,int y1,int x2,int y2,int x3,int y3,
                       int* xmin,int* xmax,int* ymin,int* ymax){

    *xmin= MAX(MIN(x1,MIN(x2,x3)),0);
    *xmax= MIN(WIDTH-1,MAX(x1,MAX(x2,x3)));
    
    *ymin= MAX(MIN(y1,MIN(y2,y3)),0);
    *ymax= MIN(HEIGHT-1,MAX(y1,MAX(y2,y3)));
}


/*void point_swap(vec3* p1,vec3* p2){
    vec3 temp= *p1;
    *p1= *p2;
    *p2= temp;
}

void point_sort(vec3* pnts){
        if (pnts[0].y> pnts[1].y){
            point_swap(&pnts[0],&pnts[1]);
        }
        if (pnts[1].y> pnts[2].y){
            point_swap(&pnts[1],&pnts[2]);
        }
        if (pnts[0].y> pnts[1].y){
            point_swap(&pnts[0],&pnts[1]);
        }
}*/

/*Scanline wont work when x_left==x_right
better to draw a line instead with frontmost z*/
/*void scanline_rasterize(vector* ndc_pnts,uint32_t face_color1,uint32_t face_color2){
    point_sort(ndc_pnts);

    int y0= screen_y(ndc_pnts[0].y),y1= screen_y(ndc_pnts[1].y),y2= screen_y(ndc_pnts[2].y);
    int x0= screen_x(ndc_pnts[0].x),x1= screen_x(ndc_pnts[1].x),x2= screen_x(ndc_pnts[2].x);
    float z0= ndc_pnts[0].z,z1= ndc_pnts[1].z,z2= ndc_pnts[2].z;

    if(y2== y0) {printf("Straight line or point");return;}           
    int y_diff= y1- y0;

    float t= (float)(y_diff)/(y2- y0);
    int split_pnt_x= x0+ t* (x2- x0);
    float split_pnt_z= z0+ t* (z2- z0);

    float m_inv,m_inv_split,mz_inv,mz_inv_split;
    float m_start,m_end;
    float mz_start,mz_end;

    float z_start,z_end,z,z_step= 0;
    float x_left,x_right;
    int x_start,x_end;
    int y_start,y_end;
    int loc;

    if (y1!= y0 || y1>= 0){
        //for top half
        m_inv= (float)(x1- x0)/(y_diff);
        m_inv_split= (float)(split_pnt_x- x0)/(y_diff);
        mz_inv= (float)(z1- z0)/(y_diff);
        mz_inv_split= (float)(split_pnt_z- z0)/(y_diff);

        if (x1> split_pnt_x){
            m_start= m_inv_split;
            m_end= m_inv;
            mz_start= mz_inv_split;
            mz_end= mz_inv;
        }
        else{
            m_start= m_inv;
            m_end= m_inv_split;
            mz_start= mz_inv;
            mz_end= mz_inv_split;
        }

        y_start= MAX(0,y0);
        y_end=  MIN(y1,HEIGHT);
        x_left= x0;
        x_right= x0;
        z_start= z0;
        z_end= z0;

        if (y0< 0){
            int y_skipped= y_start- y0;
            x_left+= m_start* y_skipped;
            x_right+= m_end* y_skipped;
            z_start+= mz_start* y_skipped;
            z_end+= mz_end* y_skipped;
        }
        for(int y= y_start;y< y_end;y++){
            if (x_right-x_left > 0.00001f){
                z_step= (z_end- z_start)/(x_right- x_left);
            }
            else{
                z_start= MAX(z_start,z_end);
                z_step= 0;
            }

            x_start= MAX(0,(int)x_left);
            x_end= MIN((int)x_right,WIDTH- 1);
            z= z_start;
            for(int x= x_start;x<= x_end;x++){
                loc= y*WIDTH+x;

                if (z> z_buffer[loc]){
                    pixels[loc]= face_color1;
                    z_buffer[loc]= z;
                }
                z+= z_step;
            }
            if (y== y_end-1){break;}
            x_left+= m_start;
            x_right+= m_end;
            z_start+= mz_start; 
            z_end+= mz_end;
        }
    }

    if (y1!= y2 || y1< HEIGHT){
        //for bottom half
        y_diff= y2- y1;

        m_inv= (float)(x2- x1)/(y_diff);
        m_inv_split= (float)(x2- split_pnt_x)/(y_diff);
        mz_inv= (float)(z2- z1)/(y_diff);
        mz_inv_split= (float)(z2- split_pnt_z)/(y_diff);

        if (x1< split_pnt_x){
            x_left= x1;
            x_right= split_pnt_x;
            z_start= z1;
            z_end= split_pnt_z;
            m_start= m_inv;
            m_end= m_inv_split;
            mz_start= mz_inv;
            mz_end= mz_inv_split;
        }
        else{
            x_left= split_pnt_x;
            x_right= x1;
            z_start= split_pnt_z;
            z_end= z1;
            m_start= m_inv_split;
            m_end= m_inv;
            mz_start= mz_inv_split;
            mz_end= mz_inv;
        }

        y_start= MAX(0,y1);
        y_end=  MIN(y2,HEIGHT);

        if (y1< 0){
            int y_skipped= y_start- y1;
            x_left+= m_start* y_skipped;
            x_right+= m_end* y_skipped;
            z_start+= mz_start* y_skipped;
            z_end+= mz_end* y_skipped;
        }
        for(int y= y_start;y< y_end;y++){
            x_left+= m_start; 
            x_right+= m_end;
            z_start+= mz_start; 
            z_end+= mz_end;
            if (x_right-x_left > 0.0001f){
                z_step= (z_end- z_start)/(x_right- x_left);
            }
            else{
                z_start= MAX(z_start,z_end);
                z_step= 0;
            }

            x_start= MAX(0,(int)x_left);
            x_end= MIN((int)x_right,WIDTH- 1);
            z= z_start;
            //printf("%f %f \n",x_left,x_right);

            for(int x= x_start;x<= x_end;x++){
                loc= y*WIDTH+x;
                
                if (z> z_buffer[loc]){
                    pixels[loc]= face_color2;
                    z_buffer[loc]= z;
                }
                z+= z_step;
            }
        }
    } 
}*/

void set_pixel(int x,int y,float ooz,uint32_t* color){
        int loc= y*WIDTH+x;
        if (ooz> z_buffer[loc]){
            printf("pixel %d\n",loc);
            pixels[loc]= *color;
            z_buffer[loc]= ooz;
        }
}