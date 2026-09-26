#include <stdio.h>
#include <stdint.h>
#include "../include/utils.h"
#include "../include/matrix.h"
#include "../include/shader.h"

//Goes over entire vertex array, gives projected ones
void vertex_FlatShader(
        VertexInput* inVertices,VertexOutput* outVertices,
        FlatShader* shader,int v_count){

    for (int i= 0;i < v_count;i++){

        outVertices[i].clip_pos= mat_mult_vec(shader->PVM,inVertices[i].pos);
        //Color between 0-255
        outVertices[i].vertex_color= shader->vertex_colors[i];
    }
}

//Will be called for each face
uint32_t fragment_FlatShader(
        float intensity,
        uint32_t vcolor1,uint32_t vcolor2,uint32_t vcolor3){

    float r= (float)((vcolor1 >> 24 & 0xFF) + (vcolor2 >> 24 & 0xFF) + (vcolor3 >> 24 & 0xFF));
    float g= (float)((vcolor1 >> 16 & 0xFF) + (vcolor2 >> 16 & 0xFF) + (vcolor3 >> 16 & 0xFF));
    float b= (float)((vcolor1 >> 8 & 0xFF) + (vcolor2 >> 8 & 0xFF) + (vcolor3 >> 8 & 0xFF));
    float a= (float)((vcolor1 & 0xFF) + (vcolor2 & 0xFF) + (vcolor3 & 0xFF));

    //Ambient,Avg
    intensity= maxf(0.2f,intensity)/3.0f;

    uint32_t rr= (uint32_t)(minf(r*intensity,255.0f));
    uint32_t gg= (uint32_t)(minf(g*intensity,255.0f));
    uint32_t bb= (uint32_t)(minf(b*intensity,255.0f));
    uint32_t aa= (uint32_t)(minf(a/3.0f,255.0f));

    return (rr<<24) | (gg<<16) | (bb<<8) | aa;
}

void vertex_GouraudShader(
        VertexInput* inVertices,VertexOutput* outVertices,
        GouraudShader* shader,int v_count){

    for (int i= 0;i < v_count;i++){

        outVertices[i].clip_pos= mat_mult_vec(shader->PVM,inVertices[i].pos);
	    outVertices[i].norm= mat_mult_vec(shader->N,inVertices[i].norm);
        //Color between 0-255
        outVertices[i].vertex_color= shader->vertex_colors[i];
    }
}

//Will be called for each pixel
uint32_t fragment_GouraudShader(Color interp,float diffuse){

    //Specular
    diffuse= minf(diffuse,1.0f);
    //Ambient
    diffuse= maxf(0.2f,diffuse);

    uint32_t rr= (uint32_t)(minf(interp.r*diffuse,255.0f));
    uint32_t gg= (uint32_t)(minf(interp.g*diffuse,255.0f));
    uint32_t bb= (uint32_t)(minf(interp.b*diffuse,255.0f));
    uint32_t aa= (uint32_t)(minf(interp.a,255.0f));

    uint32_t color= (rr<<24) | (gg<<16) | (bb<<8) | aa;
    return color;
}
