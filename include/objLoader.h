#ifndef OBJLOADER_H
#define OBJLOADER_H

#include "obj.h"

Mesh* load_Object(const char* filename, uint32_t defaultColor);

void free_Mesh(Mesh* mesh);

#endif
