#include <stdio.h>
#include <stdbool.h>
#include "../include/obj.h"
#include "../include/rasterizer.h"
#include "../include/buffer.h"

#define MAX_RENDER_VERTICES 10000
static VertexOutput g_vertexBuffer[MAX_RENDER_VERTICES];

bool render_Object(Framebuffer* buffer,mat4* PV,Object* obj,vec3 light_dir,Shademode mode){

    if (!obj || !obj->mesh_data || !PV){ 
        fprintf(stderr, "Error: Null pointer passed to render function\n");
        return false;
    }
    if (obj->mesh_data->vertex_count <= 0 || obj->mesh_data->face_count <= 0) {
        return false; 
    }

    Mesh* mesh= obj->mesh_data;

    if (mesh->vertex_count > MAX_RENDER_VERTICES) {
        fprintf(stderr, "Error: Mesh exceeds scratch vertex buffer size!\n");
        return false;
    }

    //Fresh PVM
    mat4 PVM,N;
    build_ModelMatrix(obj);
    build_NormalMatrix(&N,&obj->model);
    mat_mult_mat(PV,&obj->model,&PVM); 

    //Vertex shader
    Shader shader;
    if(mode == SHADE_FLAT){
        shader.Flat.light_dir= light_dir;
        shader.Flat.M= &obj->model;  //could be MV
        shader.Flat.PVM= &PVM;
        vertex_FlatShader(mesh->vertices,g_vertexBuffer,&shader.Flat,mesh->vertex_count);
    }
    else if(mode == SHADE_GOURAUD){
        shader.Gouraud.light_dir= light_dir;
        shader.Gouraud.M= &obj->model;  //could be MV
        shader.Gouraud.PVM= &PVM;
        shader.Gouraud.N= &N;
        vertex_GouraudShader(mesh->vertices,g_vertexBuffer,&shader.Gouraud,mesh->vertex_count);
    }

    //Primitive Assembly
    int num= mesh->face_count;
    Face* faces= mesh->faces;
    VertexOutput faceVertices[3];
    for (int i= 0;i< num;i++){

        faceVertices[0]= g_vertexBuffer[faces[i].vertices_index[0]];
        faceVertices[1]= g_vertexBuffer[faces[i].vertices_index[1]];
        faceVertices[2]= g_vertexBuffer[faces[i].vertices_index[2]];

        if(mode == SHADE_FLAT){
	        shader.Flat.face_norm= mat_mult_vec(&N,faces[i].face_norm);
            rasterize_barycentricFlat(buffer,faceVertices,&shader.Flat);
        }
        else if(mode == SHADE_GOURAUD){
            rasterize_barycentricGouraud(buffer,faceVertices,&shader.Gouraud);
        }
    }
    return true;
}

//TODO: Frustrum,z clipping after primitive
