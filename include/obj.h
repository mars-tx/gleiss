#ifndef OBJ_H
#define OBJ_H

#include "vector.h"

typedef struct{
    float m[4][4];
}mat4;

typedef struct{
    vec3 pos;
    vec3 norm;
    float u,v;
}vertex;

typedef struct{
    int vx_index[3];
    vec3 norm;

}face;

typedef struct{
    vertex* vx_all;
    face* faces;

    int vx_count;
    int face_count;
    
}mesh;

typedef struct{
    mesh* mesh_data;
    vertex* vx_trans;

    mat4 model;
    vec3 pos;
    vec3 scale;
    vec3 rot;
}object;

typedef struct{
    mesh* mesh_data;

    mat4 model;
    vec3 pos;
    vec3 scale;
    vec3 rot;
}dynamicObject;

typedef struct{
    mat4 view;
    mat4 projection;
    vec3 target;
    vec3 cam_pos;
    vec3 global;

}camera;

#endif