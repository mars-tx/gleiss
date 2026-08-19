#ifndef OBJ_H
#define OBJ_H

#include <stdint.h>
#include "vector.h"
#include "matrix.h"
#include "shader.h"

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

    mat4 model;
    vec3 pos;
    vec3 scale;
    vec3 rot;
}Object;

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

void build_ModelMatrix(Object* obj);

void build_NormalMatrix(mat4* normMat,mat4* modelMat);

void build_ViewLookAtMatrix(Camera* cam);

void build_ProjectionMatrix(Camera* cam);

#endif
