#ifndef UTILS_H
#define UTILS_H

#define Z_NEAR 0.1f
#define Z_FAR 100.0f

#define MAX(x,y)    (((x)>= (y))? (x) : (y))
#define MIN(x,y)    (((x)<= (y))? (x) : (y))

#include "vector.h"

typedef struct{
    float m[4][4];

}mat4;

vec3 mat_mult_vec(mat4* matrix,vec3 vec);

void mat_mult_mat(mat4* a,mat4* b,mat4* out);

#endif
