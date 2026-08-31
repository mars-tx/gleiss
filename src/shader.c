#include <stdio.h>
#include <stdint.h>
#include "../include/utils.h"
#include "../include/vector.h"
#include "../include/matrix.h"
#include "../include/shader.h"

//Goes over entire vertex array, gives projected ones
void vertex_FlatShader(
        VertexInput* inVertices,
        VertexOutput* outVertices,
        FlatShader* shader,int vertexCount){

    for (int i= 0;i< vertexCount;i++){

        //outVertices[i].worldPos= mat_mult_vec(shader->M,inVertices[i].pos);
        outVertices[i].clipPos= mat_mult_vec(shader->PVM,inVertices[i].pos);

        //Color between 0-255
        uint32_t vertex_color= shader->input_colors[i];
        outVertices[i].vertex_color.r= ((vertex_color>>24) & 0xff);
        outVertices[i].vertex_color.g= ((vertex_color>>16) & 0xff);
        outVertices[i].vertex_color.b= ((vertex_color>>8) & 0xff);
        outVertices[i].vertex_color.a= ((vertex_color) & 0xff);
    }
}

//Will be called for each face
//Early z testing will clear out unneeded calls
uint32_t fragment_FlatShader(
        FlatShader* shader,
        Color vcolor1,Color vcolor2,Color vcolor3){

    float div= 1.0f/3.0f;
    float r= (vcolor1.r + vcolor2.r + vcolor3.r)*div;
    float g= (vcolor1.g + vcolor2.g + vcolor3.g)*div;
    float b= (vcolor1.b + vcolor2.b + vcolor3.b)*div;
    float a= (vcolor1.a + vcolor2.a + vcolor3.a)*div;

    float diffuse= vec3Dot(shader->face_norm,shader->light_dir);
    diffuse= maxf(0.2f,minf(diffuse,1.0f));

    r*= diffuse;
    g*= diffuse;
    b*= diffuse;

    uint8_t rr= (uint8_t)(minf(r,255.0f));
    uint8_t gg= (uint8_t)(minf(g,255.0f));
    uint8_t bb= (uint8_t)(minf(b,255.0f));
    uint8_t aa= (uint8_t)(minf(a,255.0f));

    uint32_t flatColor= (rr<<24) | (gg<<16) | (bb<<8) | aa;
    return flatColor;
}

void vertex_GouraudShader(
        VertexInput* inVertices,
        VertexOutput* outVertices,
        GouraudShader* shader,int vertexCount){

    for (int i= 0;i< vertexCount;i++){

        //outVertices[i].worldPos= mat_mult_vec(shader->M,inVertices[i].pos);
        outVertices[i].clipPos= mat_mult_vec(shader->PVM,inVertices[i].pos);
	    outVertices[i].norm= mat_mult_vec(shader->N,inVertices[i].norm);

        //Color between 0-255
        uint32_t vertex_color= shader->input_colors[i];
        outVertices[i].vertex_color.r= ((vertex_color>>24) & 0xff);
        outVertices[i].vertex_color.g= ((vertex_color>>16) & 0xff);
        outVertices[i].vertex_color.b= ((vertex_color>>8) & 0xff);
        outVertices[i].vertex_color.a= ((vertex_color) & 0xff);
    }
}

//Will be called for each pixel
uint32_t fragment_GouraudShader(
        GouraudShader* shader,
        Color interp,float u1,float u2){

    float diffuse= u1* (shader->i1 - shader->i3) + u2* (shader->i2 - shader->i3) + shader->i3;
    diffuse= maxf(0.2f,minf(diffuse,1.0f));

    interp.r*= diffuse;
    interp.g*= diffuse;
    interp.b*= diffuse;

    uint8_t rr= (uint8_t)(minf(interp.r,255.0f));
    uint8_t gg= (uint8_t)(minf(interp.g,255.0f));
    uint8_t bb= (uint8_t)(minf(interp.b,255.0f));
    uint8_t aa= (uint8_t)(minf(interp.a,255.0f));

    uint32_t color= (rr<<24) | (gg<<16) | (bb<<8) | aa;
    return color;
}
