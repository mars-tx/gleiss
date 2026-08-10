#ifndef MATRIX_H
#define MATRIX_H

#include "obj.h"

void create_model_mat(object* obj);
void create_view_lookat_mat(camera* cam);
void create_proj_mat(camera* cam);

void mat_mult_mat(mat4* a,mat4* b,mat4* out);
vec3 mat_mult_vec(mat4* matrix,vec3 vec);

#endif