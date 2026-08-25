#ifndef VECTOR_H
#define VECTOR_H

#include "math.h"
#include "utils.h"

static inline vec3 vec3Sub(vec3 v1,vec3 v2){
    vec3 out;
    out.x= v1.x - v2.x; out.y= v1.y - v2.y; out.z= v1.z - v2.z;
    return out;
}

static inline float vec3Dot(vec3 v1,vec3 v2){

    float res= v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
    return res;
}

static inline vec3 vec3Cross(vec3 v1,vec3 v2){

    vec3 out;
    out.x= v1.y*v2.z - v1.z*v2.y;
    out.y= -v1.x*v2.z + v1.z*v2.x;
    out.z= v1.x*v2.y - v1.y*v2.x;
    return out;
}

static inline vec3 vec3Normalize(vec3 v){

    float magn= sqrtf(vec3Dot(v,v));
    if (magn > 0.0001f){
        magn= (1.0f/magn);
        v.x*=magn;
        v.y*=magn;
        v.z*=magn;
    }
    return v;
}

#endif
