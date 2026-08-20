#ifndef RENDER_H
#define RENDER_H

#include <stdbool.h>
#include "obj.h"
#include "buffer.h"

bool render_Object(Framebuffer* buffer, mat4* PV, Object* obj, vec3 lightDir, Shademode mode);

#endif
