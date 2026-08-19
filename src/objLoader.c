#include <stdio.h>
#include <stdlib.h>
#include "../include/obj.h"
#include "../include/vector.h"

void set_MeshFacesNormal(Mesh* inMesh) {
    int num= inMesh->faceCount;
    VertexInput* vertices= inMesh->vertices;
    Face* faces= inMesh->faces;

    for (int i= 0; i < num; i++) {
        vec3 v1= vertices[faces[i].verticesIndex[0]].pos;
        vec3 v2= vertices[faces[i].verticesIndex[1]].pos;
        vec3 v3= vertices[faces[i].verticesIndex[2]].pos;

        vec3 e1= vec3Sub(v2, v1);
        vec3 e2= vec3Sub(v3, v1);

        faces[i].faceNorm= vec3Normalize(vec3Cross(e1, e2));
    }
}

void set_MeshVerticesNormal(Mesh* inMesh){

    int num= inMesh->vertexCount;
    VertexInput* vertices= inMesh->vertices;
    for (int i= 0;i< num;i++){

        vertices[i].norm= vec3Normalize(vertices[i].pos);
    }
}

Mesh* load_Object(const char* filename, uint32_t defaultColor) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr,"Failed to open file");
        return NULL;
    }

    int v_cap = 100, f_cap = 100;
    int v_count = 0, f_count = 0;

    VertexInput* vertices = malloc(v_cap * sizeof(VertexInput));
    Face* faces = malloc(f_cap * sizeof(Face));

    if (!vertices || !faces) {
        fprintf(stderr, "Out of memory allocating mesh buffers\n");
        fclose(file); 
        free(vertices); 
        free(faces);
        return NULL;
    }

    char line[128];
    while (fgets(line, sizeof(line), file)) {
        // Parse Vertex Positions and Colors (v x y z [r g b])
        if (line[0] == 'v' && line[1] == ' ') {
            if (v_count >= v_cap) {
                v_cap *= 2;
                vertices = realloc(vertices, v_cap * sizeof(VertexInput));
            }

            float x, y, z;
            float r = -1.0f, g = -1.0f, b = -1.0f;

            // Try reading position AND RGB color from line (v x y z r g b)
            int parsed = sscanf(line, "v %f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);

            vertices[v_count].pos = (vec3){x, y, z};
            vertices[v_count].norm = (vec3){0, 0, 0};

            if (parsed == 6 && r >= 0.0f) {
                //Per-vertex RGB floats (0 to 1) to uint32_t RGBA
                uint8_t ur= (uint8_t)(r * 255.0f);
                uint8_t ug= (uint8_t)(g * 255.0f);
                uint8_t ub= (uint8_t)(b * 255.0f);
                vertices[v_count].vertexColor = (ur << 24) | (ug << 16) | (ub << 8) | 0xFF;
            } else {
                // Fallback 
                vertices[v_count].vertexColor = defaultColor;
            }

            v_count++;
        } 
        // Parse Faces (f v1 v2 v3)
        else if (line[0] == 'f' && line[1] == ' ') {
            if (f_count >= f_cap) {
                f_cap *= 2;
                faces = realloc(faces, f_cap * sizeof(Face));
            }

            int v1, v2, v3;
            // Parses 'f v1/vt1/vn1 ...' or 'f v1//vn1 ...' or plain 'f v1 v2 v3'
            if (sscanf(line, "f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &v1, &v2, &v3) != 3 &&
                sscanf(line, "f %d//%*d %d//%*d %d//%*d", &v1, &v2, &v3) != 3 &&
                sscanf(line, "f %d %d %d", &v1, &v2, &v3) != 3) {
                continue;
            }

            // Convert 1-based indexing to 0-based indices
            faces[f_count].verticesIndex[0] = v1 - 1;
            faces[f_count].verticesIndex[1] = v2 - 1;
            faces[f_count].verticesIndex[2] = v3 - 1;
            f_count++;
        }
    }
    fclose(file);

    Mesh* mesh = malloc(sizeof(Mesh));
    mesh->vertices = vertices;
    mesh->faces = faces;
    mesh->vertexCount = v_count;
    mesh->faceCount = f_count;

    // Build per-face and per-vertex normals
    set_MeshFacesNormal(mesh);
    set_MeshVerticesNormal(mesh);

    return mesh;
}

void free_Mesh(Mesh* mesh) {
    if (mesh) {
        if (mesh->vertices) free(mesh->vertices);
        if (mesh->faces) free(mesh->faces);
        free(mesh);
    }
}
