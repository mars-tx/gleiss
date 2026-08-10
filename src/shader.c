#include <stdio.h>
#include <stdlib.h>
#include "include/matrix.h"
#include "include/shader.h"

//Goes over entire vertex array, gives projected ones
void vertex_FlatShader(VertexInput* inVertices,FlatShader* shader,int vertexCount){

    VertexOutput* outVertices= malloc(sizeof(VertexInput)*vertexCount);
    for (int i= 0;i< vertexCount;i++){

        outVertices[i].world_pos= mat_mult_vec(shader->M,inVertices[i].pos);
        outVertices[i].clip_pos= mat_mult_vec(shader->PVM,inVertices[i].pos);
    }
}

//Will be called for each pixel
//Early z testing will clear out unneeded calls
void fragment_FlatShader(FlatShader* shader){
}

void vertex_GoraudShader(VertexInput* inVertices,GouraudShader* shader,int vertexCount){

    VertexOutput* outVertices= malloc(sizeof(VertexInput)*vertexCount);
    for (int i= 0;i< vertexCount;i++){

        outVertices[i].world_pos= mat_mult_vec(shader->M,inVertices[i].pos);
        outVertices[i].clip_pos= mat_mult_vec(shader->PVM,inVertices[i].pos);
        //TODO: Also do norm
    }
}

void fragment_GouraudShader(float u1,float u2,float u3,
                            GouraudShader* shader){

    float i= u1* shader->i1 + u2* shader->i2 + u3* shader->i3;
    //return rgbval
}