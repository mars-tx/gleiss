#ifndef LOADER_H
#define LOADER_H

#include "obj.h"

Mesh* load_Object(const char* filename, uint32_t default_color);

void free_Mesh(Mesh* mesh);

#endif
