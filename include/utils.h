#ifndef UTILS_H
#define UTILS_H

#define Z_NEAR 0.1f
#define Z_FAR 100.0f

static inline float maxf(float a, float b){
    return (a > b) ? a : b;
}

static inline float minf(float a, float b){
    return (a < b) ? a : b;
}

#define FABS(x)    (((x)< 0.0f)? (-x) : (x))

// Type definitions
typedef struct{
    float x,y;

}vec2;

typedef struct{
    float x,y,z;

}vec3;

typedef struct{
    float m[4][4];

}mat4;

#endif
