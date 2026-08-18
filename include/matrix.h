#ifndef MATRIX_H
#define MATRIX_H

#include "obj.h"

typedef struct{
    float mat[4][4];

}mat4;

void build_ModelMatrix(Object* obj);
voud build_NormallMatrix(mat4* normMat,mat4* modelMat);
void build_ViewLookAtMatrix(Camera* cam);
void build_ProjectionMatrix(Camera* cam);

vec3 mat_mult_vec(mat4* matrix,vec3 vec);
void mat_mult_mat(mat4* a,mat4* b,mat4* out);

#endif
