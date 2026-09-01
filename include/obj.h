#ifndef OBJ_H
#define OBJ_H

#include <stdint.h>
#include "utils.h"
#include "shader.h"

typedef struct{
    int vertices_index[3];
    vec3 face_norm;

}Face;

typedef struct{
    VertexInput* vertices;
    Face* faces;

    int vertex_count;
    int face_count;
    
}Mesh;

typedef struct{
    Mesh* mesh_data;
    uint32_t* vertex_colors;

    mat4 model;
    vec3 position;
    vec3 scale;
    vec3 rot;

}Object;

typedef struct{
    mat4 view;
    mat4 projection;

    //For view matrix
    vec3 target;
    vec3 position;
    vec3 global;

    //In radians
    float yFov;

}Camera;

#endif
