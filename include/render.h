#ifndef RENDER_H
#define RENDER_H

#include <stdbool.h>
#include "matrix.h"`

bool render_StaticObject(mat4* PV,Object* obj,vec3 lightDir,Shademode mode);
bool render_DynamicObject(mat4* PV,DynamicObject* obj,vec3 lightDir,Shademode mode);

#endif