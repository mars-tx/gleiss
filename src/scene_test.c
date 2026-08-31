#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/vector.h"
#include "../include/Scene.h"

#define MAX_VERTICES 10000

static void set_MeshFacesNormal(Mesh* inMesh) {
    int num= inMesh->face_count;
    VertexInput* vertices= inMesh->vertices;
    Face* faces= inMesh->faces;

    for (int i= 0; i < num; i++) {
        vec3 v1= vertices[faces[i].vertices_index[0]].pos;
        vec3 v2= vertices[faces[i].vertices_index[1]].pos;
        vec3 v3= vertices[faces[i].vertices_index[2]].pos;

        vec3 e1= vec3Sub(v2, v1);
        vec3 e2= vec3Sub(v3, v1);

        faces[i].face_norm= vec3Normalize(vec3Cross(e1, e2));
    }
}

static void set_MeshVerticesNormal(Mesh* inMesh){
    int num= inMesh->vertex_count;
    VertexInput* vertices= inMesh->vertices;
    for (int i= 0;i< num;i++){

        vertices[i].norm= vec3Normalize(vertices[i].pos);
    }
}

Scene* load_Scene(const char* filename){
    FILE* file= fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Failed to open scene file: %s\n", filename);
        return NULL;
    }

    Scene* scene= malloc(sizeof(Scene));
    if (!scene){
        fclose(file);
        return NULL;
    }

    //Read Counts
    if (fscanf(file, "%d %d", &scene->mesh_count, &scene->object_count) != 2){
        fclose(file);
        free(scene);
        return NULL;
    }

    scene->meshes= malloc(scene->mesh_count * sizeof(Mesh));
    scene->objects= malloc(scene->object_count * sizeof(Object));

    //Load Meshes by Index
    for (int m= 0; m< scene->mesh_count; m++) {

        int v_count, f_count;
        fscanf(file, "%d %d", &v_count, &f_count);

        if (v_count <= 0 || f_count <= 0 || v_count > MAX_VERTICES){
            fprintf(stderr, "Error: Invalid vertex or face count!\n");
            fclose(file);
            free_Scene(scene);
            return NULL;
        }

        VertexInput* vertices= malloc(v_count * sizeof(VertexInput));
        for (int i= 0; i < v_count; i++) {
            fscanf(file, "%f %f %f", &vertices[i].pos.x, &vertices[i].pos.y, &vertices[i].pos.z);
            vertices[i].norm= (vec3){0, 0, 0};
        }

        Face* faces= malloc(f_count * sizeof(Face));
        for (int i= 0; i < f_count; i++) {
            int v1, v2, v3;
            fscanf(file, "%d %d %d", &v1, &v2, &v3);
            faces[i].vertices_index[0]= v1 - 1;
            faces[i].vertices_index[1]= v2 - 1;
            faces[i].vertices_index[2]= v3 - 1;
        }

        scene->meshes[m].vertices= vertices;
        scene->meshes[m].faces= faces;
        scene->meshes[m].vertex_count= v_count;
        scene->meshes[m].face_count= f_count;

        set_MeshFacesNormal(&scene->meshes[m]);
        set_MeshVerticesNormal(&scene->meshes[m]);
    }

    //Load Objects by Index
    for (int o= 0; o < scene->object_count; o++) {

        int mesh_idx;
        float px, py, pz, rx, ry, rz, sx, sy, sz;
        
        fscanf(file, "%d %f %f %f %f %f %f %f %f %f", 
               &mesh_idx, &px, &py, &pz, &rx, &ry, &rz, &sx, &sy, &sz);

        scene->objects[o].mesh_data= &scene->meshes[mesh_idx];

        scene->objects[o].position= (vec3){px, py, pz};
        scene->objects[o].rot= (vec3){rx, ry, rz};
        scene->objects[o].scale= (vec3){sx, sy, sz};

        int v_cnt= scene->meshes[mesh_idx].vertex_count;
        uint32_t* colors= malloc(v_cnt * sizeof(uint32_t));
        for (int i= 0; i < v_cnt; i++) {
            uint8_t r, g, b;
            fscanf(file, "%hhu %hhu %hhu", &r, &g, &b);
            colors[i]= (r << 24) | (g << 16) | (b << 8) | 0xFF;
        }
        scene->objects[o].vertex_colors= colors;
    }

    //Load Camera
    fscanf(file, "%f %f %f %f %f %f %f %f %f %f",
           &scene->camera.position.x, &scene->camera.position.y, &scene->camera.position.z,
           &scene->camera.target.x, &scene->camera.target.y, &scene->camera.target.z,
           &scene->camera.global.x, &scene->camera.global.y, &scene->camera.global.z,
           &scene->camera.yFov);

    fclose(file);
    return scene;
}

void free_Scene(Scene* scene){
    if (!scene) return;

    //Free shared meshes
    if (scene->meshes) {
        for (int i= 0; i< scene->mesh_count; i++) {
            if (scene->meshes[i].vertices) {
                free(scene->meshes[i].vertices);
            }
            if (scene->meshes[i].faces) {
                free(scene->meshes[i].faces);
            }
        }
        free(scene->meshes);
    }
    //Free objects and their colors
    if (scene->objects) {
        for (int i= 0; i< scene->object_count; i++) {
            if (scene->objects[i].vertex_colors) {
                free(scene->objects[i].vertex_colors);
            }
        }
        free(scene->objects);
    }
    free(scene);
}
