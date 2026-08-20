#ifndef OBJ_H
#define OBJ_H

#include <stdint.h>
#include "vector.h"
#include "matrix.h"
#include "shader.h"
#include "buffer.h"

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

void build_ModelMatrix(Object* obj);

void build_NormalMatrix(mat4* normMat,mat4* modelMat);

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

void build_ViewLookAtMatrix(Camera* cam);

void build_ProjectionMatrix(Camera* cam,Framebuffer* fb);

#endif
