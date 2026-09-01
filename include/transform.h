#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "obj.h"
#include "scene.h"
#include "engine.h"

void build_ModelMatrix(Object* obj);

void build_NormalMatrix(mat4* norm_mat,mat4* model_mat);

void build_ViewLookAtMatrix(Scene* scene);

void build_ProjectionMatrix(EngineContext* engine,Scene* scene);

void build_ProjectionViewMatrix(Scene* scene);

#endif
