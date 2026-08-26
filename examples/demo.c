#include <stdio.h>
#include "../include/engine.h"
#include "../include/matrix.h"
#include "../include/obj.h"
#include "../include/loader.h"
#include "../include/shader.h"
#include "../include/render.h"
#include "../include/buffer.h"

int main(int argc, char* argv[]) {
    // Allow loading any custom object via command line, fallback to cube
    const char* objPath = (argc > 1) ? argv[1] : "assets/cube.obj";

    EngineContext* engine = create_Engine(800, 600, "zzz");
    if (!engine) {
        printf("Error: Failed to create engine context.\n");
        return 1;
    }

    // Setup Camera and Projections
    Camera cam;
    cam.position = (vec3){0.0f, 0.0f,  0.0f};
    cam.target   = (vec3){0.0f, 0.0f,  10.0f};
    cam.global   = (vec3){0.0f, 1.0f,  0.0f};
    cam.yFov     = 1.5708f; // 90 degrees in radians

    build_ViewLookAtMatrix(&cam);
    build_ProjectionMatrix(&cam, engine->framebuffer);

    mat4 PV;
    mat_mult_mat(&cam.projection, &cam.view, &PV);

    // Load 3D Mesh Asset
    Mesh* testMesh = load_Object(objPath, 0xFFFFFFFF);
    if (!testMesh) {
        destroy_Engine(engine);
        return 1;
    }

    Object modelObj;
    modelObj.mesh_data = testMesh;
    modelObj.position  = (vec3){0.0f, 0.0f, 4.0f};
    modelObj.rot       = (vec3){0.0f, 0.0f, 0.0f};
    modelObj.scale     = (vec3){1.0f, 1.0f, 1.0f};

    vec3 light_dir = {0.0f, 0.0f, 1.0f};

    // Main Engine Loop
    while (engine->isRunning) {

        // Clear both pixel color and depth buffers
        clear_Framebuffer(engine->framebuffer, 0x000000FF);

        // Animate object rotation
        modelObj.rot.y += 0.02f;
        modelObj.rot.x += 0.01f;

        // Render object to the framebuffer using Gouraud shading
        render_Object(engine->framebuffer, &PV, &modelObj, light_dir, SHADE_GOURAUD);

        // Blit buffer to texture and present renderer
        update_Engine(engine);

        SDL_Delay(16); // Cap at ~60 FPS
    }

    // Clean up resources cleanly
    free_Mesh(testMesh);
    destroy_Engine(engine);

    printf("Exited.\n");
    return 0;
}
