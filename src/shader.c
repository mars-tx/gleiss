#include <stdio.h>
#include <stdlib.h>
#include "include/matrix.h"
#include "include/shader.h"
#include "include/main.h"

//Goes over entire vertex array, gives projected ones
void vertex_FlatShader(VertexInput* inVertices,VertexOutput* outVertices,FlatShader* shader,int vertexCount){

    for (int i= 0;i< vertexCount;i++){

        outVertices[i].worldPos= mat_mult_vec(shader->M,inVertices[i].pos);
        outVertices[i].clipPos= mat_mult_vec(shader->PVM,inVertices[i].pos);

        //Color between 0-1
        float div= 1.0f/255.0f;
        outVertices[i].vertexColor.r= ((inVertices[i].vertexColor>>24) & 0xff)*div;
        outVertices[i].vertexColor.g= ((inVertices[i].vertexColor>>16) & 0xff)*div;
        outVertices[i].vertexColor.b= ((inVertices[i].vertexColor>>8) & 0xff)*div;
        outVertices[i].vertexColor.a= ((inVertices[i].vertexColor) & 0xff)*div;
    }
}

//Will be called for each pixel
//Early z testing will clear out unneeded calls
uint32_t fragment_FlatShader(FlatShader* shader,Color vcolor1,Color vcolor2,Color vcolor3){

    float div= 255.0f/3.0f;
    float r= (vcolor1.r + vcolor2.r + vcolor3.r)*div;
    float g= (vcolor1.g + vcolor2.g + vcolor3.g)*div;
    float b= (vcolor1.b + vcolor2.b + vcolor3.b)*div;
    float a= (vcolor1.a + vcolor2.a + vcolor3.a)*div;

    r*= shader->intensity;
    g*= shader->intensity;
    b*= shader->intensity;

    uint8_t rr= (uint8_t)(MIN(r,255.0f));
    uint8_t gg= (uint8_t)(MIN(g,255.0f));
    uint8_t bb= (uint8_t)(MIN(b,255.0f));
    uint8_t aa= (uint8_t)(MIN(a,255.0f));

    uint32_t flatColor= (rr<<24) | (gg<<16) | (bb<<8) | aa;
    return flatColor;
}

void vertex_GoraudShader(VertexInput* inVertices,VertexOutput* outVertices,GouraudShader* shader,int vertexCount){

    for (int i= 0;i< vertexCount;i++){

        outVertices[i].worldPos= mat_mult_vec(shader->M,inVertices[i].pos);
        outVertices[i].clipPos= mat_mult_vec(shader->PVM,inVertices[i].pos);
        //TODO: Also do norm

        //Color between 0-1
        float div= 1.0f/255.0f;
        outVertices[i].vertexColor.r= ((inVertices[i].vertexColor>>24) & 0xff)*div;
        outVertices[i].vertexColor.g= ((inVertices[i].vertexColor>>16) & 0xff)*div;
        outVertices[i].vertexColor.b= ((inVertices[i].vertexColor>>8) & 0xff)*div;
        outVertices[i].vertexColor.a= ((inVertices[i].vertexColor) & 0xff)*div;
    }
}

uint32_t fragment_GouraudShader(GouraudShader* shader,Color interp,float u1,float u2){

    float diffuse= u1* (shader->i1 - shader->i3) + u2* (shader->i2 - shader->i3) + shader->i3;
    diffuse= MAX(0.2f,MIN(diffuse,1.0f))*255.0f;

    interp.r*= diffuse;
    interp.g*= diffuse;
    interp.b*= diffuse;
    interp.a*= 255.0f;

    uint8_t rr= (uint8_t)(MIN(interp.r,255.0f));
    uint8_t gg= (uint8_t)(MIN(interp.g,255.0f));
    uint8_t bb= (uint8_t)(MIN(interp.b,255.0f));
    uint8_t aa= (uint8_t)(MIN(interp.a,255.0f));

    uint32_t color= (rr<<24) | (gg<<16) | (bb<<8) | aa;
    return color;
}