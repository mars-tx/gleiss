#ifndef MATRIX_H
#define MATRIX_H

#include "utils.h"

static inline vec3 mat_mult_vec(const mat4* matrix,vec3 vec){

   vec3 result;

   result.x= matrix->m[0][0]*vec.x + matrix->m[0][1]*vec.y + matrix->m[0][2]*vec.z 
           + matrix->m[0][3];
   result.y= matrix->m[1][0]*vec.x + matrix->m[1][1]*vec.y + matrix->m[1][2]*vec.z 
           + matrix->m[1][3];
   result.z= matrix->m[2][0]*vec.x + matrix->m[2][1]*vec.y + matrix->m[2][2]*vec.z 
           + matrix->m[2][3];

   return result;
}
static inline void mat_mult_mat(
    const mat4* restrict a,
    const mat4* restrict b,
          mat4* restrict out)  
{
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            float sum = 0.0f; 
            for (int k = 0; k < 4; k++) {
                sum += a->m[r][k] * b->m[k][c]; 
            }
            out->m[r][c] = sum; 
        }
    }
}

#endif
