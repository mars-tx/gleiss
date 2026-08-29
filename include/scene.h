#ifndef SCENE_H
#define SCENE_H

#include "obj.h"

typedef struct {
    Mesh* meshes;
    int mesh_count;

    Object* objects;
    int object_count;

    Camera camera;

}Scene;

Scene* load_Scene(const char* filename);

void free_Scene(Scene* scene);

#endif
