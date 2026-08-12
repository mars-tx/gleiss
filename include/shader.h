#ifndef SHADER_H
#define SHADER_H

#include "vector.h"

typedef enum{SHADE_FLAT,SHADE_GOURAUD}Shademode;

typedef struct{
    float r,g,b,a;

}Color;

typedef struct{
    //Per object
    mat4* PVM;
    mat4* M;

    vec3 lightDir;
    //Per face
    vec3 faceNorm;
    float intensity;

}FlatShader;

typedef struct{
    //Per object
    mat4* PVM;
    mat4* M;
    mat4* N;

    vec3 lightDir;
    //Per face vertices
    float i1,i2,i3;

}GouraudShader;

typedef struct{
    FlatShader Flat;
    GouraudShader Gouraud;

}Shader;

//Inside Mesh
typedef struct{
    uint32_t vertexColor;
    vec3 pos;
    vec3 norm;
    vec2 texture;

}VertexInput;

typedef struct{
    Color vertexColor;
    //vec3 clip_pos as z remains intact
    vec3 clipPos;
    //Transformed norm
    vec3 norm;

    vec3 worldPos;
    vec2 texture;

}VertexOutput;

void vertex_FlatShader(VertexInput* inVertices,VertexOutput* outVertices,FlatShader* shader,int vertexCount);
void vertex_GouraudShader(VertexInput* inVertices,VertexOutput* outVertices,GouraudShader* shader,int vertexCount);

uint32_t fragment_FlatShader(FlatShader* shader,Color vcolor1,Color vcolor2,Color vcolor3);
uint32_t fragment_GouraudShader(GouraudShader* shader,Color interp,float u1,float u2);

#endif