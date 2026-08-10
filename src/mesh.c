#include <stdio.h>
#include "include/obj.h"
#include "include/shader.h"
#include "include/vector.h"

void setMeshVerticesNormal(Mesh* inMesh){

    int num= inMesh->vertexCount;
    VertexInput* vertices= inMesh->vertices;
    for (int i= 0;i< num;i++){

        vertices[i].norm= vec3Normalize(vertices[i].pos);
    }
}

void setMeshFacesNormal(Mesh* inMesh){

    int num= inMesh->faceCount;
    VertexInput* vertices= inMesh->vertices;
    Face* faces= inMesh->faces;
    for (int i= 0;i< num;i++){

        vec3 v1= vertices[faces[i].verticesIndex[0]].pos;
        vec3 v2= vertices[faces[i].verticesIndex[1]].pos;
        vec3 v3= vertices[faces[i].verticesIndex[2]].pos;

        vec3 n1= vec3Sub(v2,v1);
        vec3 n2= vec3Sub(v3,v1);

        faces[i].norm= vec3Normalize(vec3Cross(n1,n2));
    }
}
