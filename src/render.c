#include <stdbool.h>
#include "include/vector.h"
#include "include/obj.h"
#include "include/matrix.h"

bool render_StaticObject(mat4* PV,Object* obj,vec3 lightDir,Shademode mode){

    Mesh* mesh= obj->meshData;
    //Static obj, vTrans cached 
    VertexOutput* vTrans= obj->verticesTransformed;

    //setMeshVerticesNormal(mesh);

    /*If PV changes,
    only then PVM and vertex shader*/
    mat4 PVM;
    build_ModelMatrix(obj);
    mat_mult_mat(PV,&obj->model,&PVM); 

    //Vertex shader
    Shader shader;
    if(mode== SHADE_FLAT){
        shader.Flat.lightDir= lightDir;
        shader.Flat.M= &obj->model;  //could be MV
        shader.Flat.PVM= &PVM;
        vertex_FlatShader(mesh->vertices,vTrans,&shader,mesh->vertexCount);
    }
    else if(mode== SHADE_GOURAUD){
        shader.Gouraud.lightDir= lightDir;
        shader.Gouraud.M= &obj->model;  //could be MV
        shader.Gouraud.PVM= &PVM;
        shader.Gouraud.N;
        vertex_GouraudShader(mesh->vertices,vTrans,&shader,mesh->vertexCount);
    }

    //Primitive Assembly
    int num= mesh->faceCount;
    Face* faces= mesh->faces;
    VertexOutput faceVertices[3];
    for (int i= 0;i< num;i++){

        faceVertices[0]= vTrans[faces[i].verticesIndex[0]];
        faceVertices[1]= vTrans[faces[i].verticesIndex[1]];
        faceVertices[2]= vTrans[faces[i].verticesIndex[2]];

        if(mode== SHADE_FLAT){
            rasterize_barycentricFlat(faceVertices,&shader.Flat);
        }
        else if(mode== SHADE_GOURAUD){
            rasterize_barycentricGouraud(faceVertices,&shader.Gouraud);
        }
    }
}

bool render_DynamicObject(mat4* PV,DynamicObject* obj,vec3 lightDir,Shademode mode){

    Mesh* mesh= obj->meshData;
    //Dynamic obj, vTrans stack allocation
    VertexOutput vTrans[mesh->vertexCount];


    /*If PV changes,
    only then PVM and vertex shader*/
    mat4 PVM;
    build_ModelMatrix(obj);
    mat_mult_mat(PV,&obj->model,&PVM); 

    //Vertex shader
    Shader shader;
    if(mode== SHADE_FLAT){
        shader.Flat.lightDir= lightDir;
        shader.Flat.M= &obj->model;  //could be MV
        shader.Flat.PVM= &PVM;
        vertex_FlatShader(mesh->vertices,vTrans,&shader,mesh->vertexCount);
    }
    else if(mode== SHADE_GOURAUD){
        shader.Gouraud.lightDir= lightDir;
        shader.Gouraud.M= &obj->model;  //could be MV
        shader.Gouraud.PVM= &PVM;
        shader.Gouraud.N;
        vertex_GouraudShader(mesh->vertices,vTrans,&shader,mesh->vertexCount);
    }

    //Primitive Assembly
    int num= mesh->faceCount;
    Face* faces= mesh->faces;
    VertexOutput faceVertices[3];
    for (int i= 0;i< num;i++){

        faceVertices[0]= vTrans[faces[i].verticesIndex[0]];
        faceVertices[1]= vTrans[faces[i].verticesIndex[1]];
        faceVertices[2]= vTrans[faces[i].verticesIndex[2]];

        if(mode== SHADE_FLAT){
            rasterize_barycentricFlat(faceVertices,&shader.Flat);
        }
        else if(mode== SHADE_GOURAUD){
            rasterize_barycentricGouraud(faceVertices,&shader.Gouraud);
        }
    }
}

//TODO: Frustrum,z clipping after primitive