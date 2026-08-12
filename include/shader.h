#ifndef SHADER_H
#define SHADER_H

#include "vector.h"

typedef enum{SHADE_FLAT,SHADE_GOURAUD}Shademode;

typedef struct{
    //Per object
    mat4* PVM;
    mat4* M;

    vec3 lightDir;
    //Per face
    vec3 faceNorm;
    float i;

}FlatShader;

typedef struct{
    //Per object
    mat4* PVM;
    mat4* M;

    vec3 lightDir;
    //Per face vertices
    float i1,i2,i3;

}GouraudShader;

typedef struct{
    //Local space
    vec3 pos;
    vec3 norm;
    vec2 texture;

}VertexInput;

typedef struct{
    //vec3 clip_pos as z remains intact
    vec3 clip_pos;
    //Transformed norm
    vec3 norm;
    //Passed along
    vec3 world_pos;
    vec2 texture;

}VertexOutput;

#endif