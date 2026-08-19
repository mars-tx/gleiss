#ifndef RENDER_H
#define RENDER_H

#include <stdbool.h>
#include "vector.h"`
#include "obj.h"

bool render_Object(mat4* PV,Object* obj,vec3 lightDir,Shademode mode);

#endif
