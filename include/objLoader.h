#ifndef OBJLOADER_H
#define OBJLOADER_H

#include "obj.h"

Mesh* load_obj(const char* filename, uint32_t defaultColor);
void free_mesh(Mesh* mesh);

#endif
