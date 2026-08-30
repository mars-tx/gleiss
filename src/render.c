#include <stdio.h>
#include <stdbool.h>
#include "../include/matrix.h"
#include "../include/scene.h"
#include "../include/rasterizer.h"
#include "../include/buffer.h"

#define MAX_RENDER_VERTICES 10000
static VertexOutput g_vertexBuffer[MAX_RENDER_VERTICES];

static bool clip_Frustrum(const VertexOutput* faceVertices);

static int clip_NearPlane(VertexOutput f_vertices[2][3]);

static inline VertexOutput interp_Vertex(VertexOutput v1, VertexOutput v2);

bool render_Scene(
        Framebuffer* buffer,
        Scene* scene,
        vec3 light_dir, Shademode mode) {

    if (!scene || !buffer) {
        fprintf(stderr, "Error: Null pointer passed\n");
        return false;
    }

    mat4* PV = &scene->ProjectionView;

    int obj_count = scene->object_count;
    for (int o = 0; o < obj_count; o++) {
    
        Object* obj = &scene->objects[o];

        Mesh* mesh = obj->mesh_data;

        // Fresh PVM & Normal matrix
        mat4 PVM, N;
        build_ModelMatrix(obj);
        build_NormalMatrix(&N, &obj->model);
        mat_mult_mat(PV, &obj->model, &PVM); 

        // Vertex shader batch pass
        Shader shader;
        if (mode == SHADE_FLAT) {
            shader.Flat.light_dir = light_dir;
            shader.Flat.PVM = &PVM;
            shader.Flat.input_colors = obj->vertex_colors;
            vertex_FlatShader(
                    mesh->vertices, g_vertexBuffer,
                    &shader.Flat, mesh->vertex_count);
        }
        else if (mode == SHADE_GOURAUD) {
            shader.Gouraud.light_dir = light_dir;
            shader.Gouraud.PVM = &PVM;
            shader.Gouraud.N = &N;
            shader.Gouraud.input_colors = obj->vertex_colors;
            vertex_GouraudShader(
                    mesh->vertices, g_vertexBuffer,
                    &shader.Gouraud, mesh->vertex_count);
        }

        // Primitive Assembly & Clipping
        int num = mesh->face_count;
        Face* faces = mesh->faces;
        VertexOutput faceVertices[2][3]; 

        for (int i = 0; i < num; i++) {
            faceVertices[0][0] = g_vertexBuffer[faces[i].vertices_index[0]];
            faceVertices[0][1] = g_vertexBuffer[faces[i].vertices_index[1]];
            faceVertices[0][2] = g_vertexBuffer[faces[i].vertices_index[2]];

            if (!clip_Frustrum(faceVertices[0])){ 
                continue; 
            }

            int tri_count = clip_NearPlane(faceVertices);
            if (tri_count == 0){
                continue;
            }

            for (int t = 0; t < tri_count; t++) {
                if (mode == SHADE_FLAT) {
                    shader.Flat.face_norm = mat_mult_vec(&N, faces[i].face_norm);
                    rasterize_barycentricFlat(faceVertices[t], &shader.Flat, buffer);
                }
                else if (mode == SHADE_GOURAUD) {
                    rasterize_barycentricGouraud(faceVertices[t], &shader.Gouraud, buffer);
                }
            }
        }
    }
    return true;
}

static inline VertexOutput interp_Vertex(VertexOutput v1, VertexOutput v2) {

    float t= (Z_NEAR - v1.clipPos.z) / (v2.clipPos.z - v1.clipPos.z);

    VertexOutput out;
    out.clipPos.x= v1.clipPos.x + t * (v2.clipPos.x - v1.clipPos.x);
    out.clipPos.y= v1.clipPos.y + t * (v2.clipPos.y - v1.clipPos.y);
    out.clipPos.z= Z_NEAR;

    out.norm.x= v1.norm.x + t * (v2.norm.x - v1.norm.x);
    out.norm.y= v1.norm.y + t * (v2.norm.y - v1.norm.y);
    out.norm.z= v1.norm.z + t * (v2.norm.z - v1.norm.z);

    out.vertex_color.r= v1.vertex_color.r + t * (v2.vertex_color.r - v1.vertex_color.r);
    out.vertex_color.g= v1.vertex_color.g + t * (v2.vertex_color.g - v1.vertex_color.g);
    out.vertex_color.b= v1.vertex_color.b + t * (v2.vertex_color.b - v1.vertex_color.b);
    out.vertex_color.a= v1.vertex_color.a + t * (v2.vertex_color.a - v1.vertex_color.a);

    out.texture.x= v1.texture.x + t * (v2.texture.x - v1.texture.x);
    out.texture.y= v1.texture.y + t * (v2.texture.y - v1.texture.y);

    return out;
}

static int clip_NearPlane(VertexOutput f_vertices[2][3]) {

    int in[3], out[3];
    int ins= 0, outs= 0;

    for (int i = 0; i < 3; i++) {
        if (f_vertices[0][i].clipPos.z >= Z_NEAR) {
            in[ins++] = i;
        } else {
            out[outs++] = i;
        }
    }

    if (ins == 0) return 0; // Completely clipped
    if (ins == 3) return 1; // Fully inside

    if (ins == 1){

        f_vertices[0][0]= f_vertices[0][in[0]];
        f_vertices[0][1]= 
            interp_Vertex(f_vertices[0][in[0]], f_vertices[0][out[0]]);
        f_vertices[0][2]= 
            interp_Vertex(f_vertices[0][in[0]], f_vertices[0][out[1]]);
        return 1;
    }

    if (ins == 2){ 
        VertexOutput vA = interp_Vertex(f_vertices[0][in[0]], f_vertices[0][out[0]]);
        VertexOutput vB = interp_Vertex(f_vertices[0][in[1]], f_vertices[0][out[0]]);

        // Face 1
        f_vertices[0][0] = f_vertices[0][in[0]];
        f_vertices[0][1] = f_vertices[0][in[1]];
        f_vertices[0][2] = vA;

        // Face 2
        f_vertices[1][0] = f_vertices[0][in[1]];
        f_vertices[1][1] = vB;
        f_vertices[1][2] = vA;

        return 2;
    }

    return 0;
}

static bool clip_Frustrum(const VertexOutput* faceVertices) {

    float z1 = faceVertices[0].clipPos.z;
    float z2 = faceVertices[1].clipPos.z;
    float z3 = faceVertices[2].clipPos.z;

    float zmax = maxf(z1,maxf(z2, z3));

    if (zmax < Z_NEAR) {
        return false;
    }
    else if (minf(z1,minf(z2, z3)) > Z_FAR) {
        return false;
    }

    float x1 = faceVertices[0].clipPos.x;
    float x2 = faceVertices[1].clipPos.x;
    float x3 = faceVertices[2].clipPos.x;

    if (minf(x1,minf(x2, x3)) > zmax) { return false; }
    else if (maxf(x1, maxf(x2, x3)) > -zmax) { return false; }

    float y1 = faceVertices[0].clipPos.y;
    float y2 = faceVertices[1].clipPos.y;
    float y3 = faceVertices[2].clipPos.y;

    if (minf(y1,minf(y2, y3)) > zmax) { return false; }
    else if (maxf(y1,maxf(y2, y3)) > -zmax) { return false; }

    return true;
}
