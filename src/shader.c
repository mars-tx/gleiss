#include "stdio.h"
#include "include/matrix.h"

void vertexFlatShader(vertex* inputVertices,mat4* PVM,int vertex_count){

    vec3 out;
    vertex* outVertices;
    for (int i= 0;i< vertex_count;i++){
        vec3 out= mat_mult_vec(PVM,inputVertices[i].pos);
}
}

//Will be called for each pixel
//Early z testing will clear out unneeded calls
void fragmentFlatShader()