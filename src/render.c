#include <stdio.h>
#include <stdbool.h>
#include "../include/matrix.h"
#include "../include/obj.h"
#include "../include/rasterizer.h"

#define MAX_RENDER_VERTICES 10000
static VertexOutput vertexBuffer[MAX_RENDER_VERTICES];

bool render_Object(mat4* PV,Object* obj,vec3 lightDir,Shademode mode){

    if (!obj || !obj->meshData || !PV){ 
        fprintf(stderr, "Error: Null pointer passed to render function\n");
        return false;
    }
    if (obj->meshData->vertexCount <= 0 || obj->meshData->faceCount <= 0) {
        return false; 
    }

    Mesh* mesh= obj->meshData;

    if (mesh->vertexCount > MAX_RENDER_VERTICES) {
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
        shader.Flat.lightDir= lightDir;
        shader.Flat.M= &obj->model;  //could be MV
        shader.Flat.PVM= &PVM;
        vertex_FlatShader(mesh->vertices,vertexBuffer,&shader.Flat,mesh->vertexCount);
    }
    else if(mode == SHADE_GOURAUD){
        shader.Gouraud.lightDir= lightDir;
        shader.Gouraud.M= &obj->model;  //could be MV
        shader.Gouraud.PVM= &PVM;
        shader.Gouraud.N= &N;
        vertex_GouraudShader(mesh->vertices,vertexBuffer,&shader.Gouraud,mesh->vertexCount);
    }

    //Primitive Assembly
    int num= mesh->faceCount;
    Face* faces= mesh->faces;
    VertexOutput faceVertices[3];
    for (int i= 0;i< num;i++){

        faceVertices[0]= vertexBuffer[faces[i].verticesIndex[0]];
        faceVertices[1]= vertexBuffer[faces[i].verticesIndex[1]];
        faceVertices[2]= vertexBuffer[faces[i].verticesIndex[2]];

        if(mode == SHADE_FLAT){
	        shader.Flat.faceNorm= mat_mult_vec(&N,faces[i].faceNorm);
            rasterize_barycentricFlat(faceVertices,&shader.Flat);
        }
        else if(mode == SHADE_GOURAUD){
            rasterize_barycentricGouraud(faceVertices,&shader.Gouraud);
        }
    }
    return true;
}

//TODO: Frustrum,z clipping after primitive
