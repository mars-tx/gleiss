#ifndef OBJ_H
#define OBJ_H

#include "vector.h"
#include "shader.h"
#include "matrix.h"

typedef struct{
    int verticesIndex[3];
    vec3 faceNorm;

}Face;

typedef struct{
    VertexInput* vertices;
    Face* faces;

    int vertexCount;
    int faceCount;
    
}Mesh;

typedef struct{
    Mesh* meshData;
    VertexOutput* verticesTransformed;

    mat4 model;
    vec3 pos;
    vec3 scale;
    vec3 rot;
}Object;

typedef struct{
    Mesh* meshData;

    mat4 model;
    vec3 pos;
    vec3 scale;
    vec3 rot;

}DynamicObject;

typedef struct{
    mat4 view;
    mat4 projection;

    //For view matrix
    vec3 target;
    vec3 cam_pos;
    vec3 global;

    //In radians
    float yFov;

}Camera;

typedef struct{
    uint32_t* pixels;
    float* z_buffer;
    int Width,Height;

}Framebuffer;

#endif
