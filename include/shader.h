#ifndef SHADER_H
#define SHADER_H

#include <stdint.h>
#include "utils.h"

typedef enum{SHADE_FLAT,SHADE_GOURAUD}Shademode;

typedef struct{
    float r,g,b,a;

}Color;

typedef struct{
    //Per object
    mat4* PVM;
    //Points to object colors
    uint32_t* input_colors;

    vec3 light_dir;
    //Per face
    vec3 face_norm;

}FlatShader;

typedef struct{
    //Per object
    mat4* PVM;
    mat4* N;
    //Points to object colors
    uint32_t* input_colors;
    vec3 light_dir;

}GouraudShader;

typedef struct{
    FlatShader Flat;
    GouraudShader Gouraud;

}Shader;

//Inside Mesh
typedef struct{
    vec3 pos;
    vec3 norm;
    vec2 texture;

}VertexInput;

typedef struct{
    //vec3 clip_pos as z remains intact
    vec3 clip_pos;
    //Transformed norm
    vec3 norm;
    vec2 texture;
    uint32_t vertex_color;

}VertexOutput;

void vertex_FlatShader(
        VertexInput* inVertices,
        VertexOutput* outVertices,
        FlatShader* shader,int vertexCount);

void vertex_GouraudShader(
        VertexInput* inVertices,
        VertexOutput* outVertices,
        GouraudShader* shader,int vertexCount);

uint32_t fragment_FlatShader(
        float intensity,
        uint32_t vcolor1,uint32_t vcolor2,uint32_t vcolor3);

uint32_t fragment_GouraudShader(Color interp,float diffuse);

#endif
