#ifndef RENDER_H
#define RENDER_H

#include <stdbool.h>
#include "scene.h"
#include "buffer.h"

typedef struct {
    Shademode mode;
    bool enable_culling;
    bool wireframe;

}RenderContext;

bool render_Scene(
        Framebuffer* buffer,
        Scene* scene,
        vec3 light_dir,Shademode mode);

#endif
