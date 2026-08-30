#ifndef SCENE_H
#define SCENE_H

#include "obj.h"

typedef struct {
    Mesh* meshes;
    Object* objects;

    mat4 ProjectionView;
    Camera camera;

    int mesh_count;
    int object_count;

}Scene;

Scene* load_Scene(const char* filename);

void free_Scene(Scene* scene);

#endif
