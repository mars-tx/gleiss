#include <stdio.h>
#include <SDL3/SDL.h>
#include "../include/engine.h"
#include "../include/scene.h"
#include "../include/matrix.h"
#include "../include/render.h"
#include "../include/buffer.h"

int main(int argc, char* argv[]) {

    EngineContext* engine = create_Engine(800, 600, "test");
    if (!engine) {
        return 1;
    }

    Scene* scene = load_Scene("assets/cube.obj");
    if (!scene) {
        fprintf(stderr, "Failed to load scene. Check file format and path.\n");
        destroy_Engine(engine);
        return 1;
    }

    vec3 lightDir = {0.0f, 0.0f, 1.0f}; 

    SDL_Event event;
    while (engine->isRunning) {
        
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                engine->isRunning = false;
            }
        }


        clear_Framebuffer(engine->framebuffer, 0x000000FF);

        build_ViewLookAtMatrix(&scene->camera);
        build_ProjectionMatrix(&scene->camera, engine->framebuffer);
        
        mat_mult_mat(&scene->camera.projection, &scene->camera.view, &scene->ProjectionView);

        render_Scene(engine->framebuffer, scene, lightDir, SHADE_GOURAUD);

        update_Engine(engine);
    }

    free_Scene(scene);
    destroy_Engine(engine);

    return 0;
}
