#include <stdio.h>
#include "include/obj.h"
#include "include/vector.h"

void setMeshVertexNormal(mesh* inMesh){

    int num= inMesh->vx_count;
    vertex* vertices= inMesh->vx_all;
    for (int i= 0;i< num;i++){
        vertices[i].norm= vec3Normalize(vertices[i].pos);
    }

}

void setMeshFaceNormal(mesh* inMesh,int mesh_count){

    int num= inMesh->face_count;
    vertex* vertices= inMesh->vx_all;
    face* faces= inMesh->faces;
    for (int i= 0;i< num;i++){
        vec3 v1= vertices[faces[i].vx_index[0]].pos;
        vec3 v2= vertices[faces[i].vx_index[1]].pos;
        vec3 v3= vertices[faces[i].vx_index[2]].pos;

        vec3 n1= vec3Sub(v2,v1);
        vec3 n2= vec3Sub(v3,v1);

        faces[i].norm= vec3Normalize(vec3Cross(n1,n2));
    }
}
