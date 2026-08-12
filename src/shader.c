#include <stdio.h>
#include <stdlib.h>
#include "include/matrix.h"
#include "include/shader.h"

//Goes over entire vertex array, gives projected ones
void vertex_FlatShader(VertexInput* inVertices,VertexOutput* outVertices,FlatShader* shader,int vertexCount){

    for (int i= 0;i< vertexCount;i++){

        outVertices[i].world_pos= mat_mult_vec(shader->M,inVertices[i].pos);
        outVertices[i].clip_pos= mat_mult_vec(shader->PVM,inVertices[i].pos);
    }
}

//Will be called for each pixel
//Early z testing will clear out unneeded calls
void fragment_FlatShader(FlatShader* shader){

    //return rgbval*intensity;
}

void vertex_GoraudShader(VertexInput* inVertices,VertexOutput* outVertices,GouraudShader* shader,int vertexCount){

    for (int i= 0;i< vertexCount;i++){

        outVertices[i].world_pos= mat_mult_vec(shader->M,inVertices[i].pos);
        outVertices[i].clip_pos= mat_mult_vec(shader->PVM,inVertices[i].pos);
        //TODO: Also do norm
    }
}

void fragment_GouraudShader(GouraudShader* shader,float u1,float u2){

    float i= u1* (shader->i1 - shader->i3) + u2* (shader->i2 - shader->i3) + shader->i3;
    //return rgbval
}