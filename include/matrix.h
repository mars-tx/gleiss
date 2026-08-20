#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"

typedef struct{
    float m[4][4];

}mat4;

vec3 mat_mult_vec(mat4* matrix,vec3 vec);

void mat_mult_mat(mat4* a,mat4* b,mat4* out);

#endif

