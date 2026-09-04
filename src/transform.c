#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../include/utils.h"
#include "../include/matrix.h"
#include "../include/obj.h"
#include "../include/scene.h"
#include "../include/engine.h"

//TODO:Quarternions implement
//Using rot vec gives gimbal

//XYZ
void build_ModelMatrix(Object* obj) {

    mat4* model_mat= &obj->model;
    vec3 pos= obj->position;
    vec3 scale= obj->scale;
    
    //Raw angles
    float ax= obj->rot.x;
    float ay= obj->rot.y;
    float az= obj->rot.z;
    
    float cx= cosf(ax); float sx= sinf(ax);
    float cy= cosf(ay); float sy= sinf(ay);
    float cz= cosf(az); float sz= sinf(az);

    //R1
    model_mat->m[0][0]= (cy * cz)* scale.x;
    model_mat->m[0][1]= (sx * sy * cz - cx * sz)* scale.y;
    model_mat->m[0][2]= (cx * sy * cz + sx * sz)* scale.z;
    model_mat->m[0][3]= pos.x;

    //R2
    model_mat->m[1][0]= (cy * sz)* scale.x;
    model_mat->m[1][1]= (sx * sy * sz + cx * cz)* scale.y;
    model_mat->m[1][2]= (cx * sy * sz - sx * cz)* scale.z;
    model_mat->m[1][3]= pos.y;

    //R3
    model_mat->m[2][0]= (-sy)* scale.x;
    model_mat->m[2][1]= (sx * cy)* scale.y;
    model_mat->m[2][2]= (cx * cy)* scale.z;
    model_mat->m[2][3]= pos.z;

    //R4
    model_mat->m[3][0]= 0.0f;
    model_mat->m[3][1]= 0.0f;
    model_mat->m[3][2]= 0.0f;
    model_mat->m[3][3]= 1.0f;

    //printf("MODELL\n");mat_print(model_mat);
}

void build_NormalMatrix(
        mat4* restrict norm_mat,
        mat4* restrict model_mat){

    float (*m)[4]= model_mat->m;

    float det;
    det= m[0][0] *(m[1][1] * m[2][2] - m[1][2] * m[2][1])
        -m[0][1] *(m[1][2] * m[2][0] - m[1][0] * m[2][2]) //sign    
        +m[0][2] *(m[1][0] * m[2][1] - m[1][1] * m[2][0]);

    //0 determinant 
    if (FABS(det) < 0.001f){
        memset(norm_mat, 0, sizeof(mat4));
        norm_mat->m[0][0] = norm_mat->m[1][1] = 
        norm_mat->m[2][2] = norm_mat->m[3][3] = 1.0f;
        return;
    }

    det = 1.0f / det;
    //Row 1 of Normal Matrix 
    norm_mat->m[0][0]= (m[1][1] * m[2][2] - m[1][2] * m[2][1])* det;
    norm_mat->m[0][1]= (m[1][2] * m[2][0] - m[1][0] * m[2][2])* det;
    norm_mat->m[0][2]= (m[1][0] * m[2][1] - m[1][1] * m[2][0])* det;
    norm_mat->m[0][3]= 0;

    //Row 2 of Normal Matrix 
    norm_mat->m[1][0]= (m[0][2] * m[2][1] - m[0][1] * m[2][2])* det; 
    norm_mat->m[1][1]= (m[0][0] * m[2][2] - m[0][2] * m[2][0])* det;
    norm_mat->m[1][2]= (m[0][1] * m[2][0] - m[0][0] * m[2][1])* det; 
    norm_mat->m[1][3]= 0;

    //Row 3 of Normal Matrix 
    norm_mat->m[2][0]= (m[0][1] * m[1][2] - m[0][2] * m[1][1])* det;
    norm_mat->m[2][1]= (m[0][2] * m[1][0] - m[0][0] * m[1][2])* det; 
    norm_mat->m[2][2]= (m[0][0] * m[1][1] - m[0][1] * m[1][0])* det;
    norm_mat->m[2][3]= 0;

    //Row 4 of Normal Matrix 
    norm_mat->m[3][0]= 0;
    norm_mat->m[3][1]= 0;
    norm_mat->m[3][2]= 0;
    norm_mat->m[3][3]= 0;
}

/*LOOKAT matrix wont work if camera is pointing straight up or down
For that its better to switch to global_z or Quaternions*/
//Left handed system
void build_ViewLookAtMatrix(Scene* scene){

    Camera* cam= &scene->camera;

    mat4* view_mat= &cam->view;
    vec3 cam_pos= cam->position;
    vec3 global= cam->global;
    float magn;

    //FORWARD,R3
    view_mat->m[2][0]= cam->target.x - cam_pos.x; 
    view_mat->m[2][1]= cam->target.y - cam_pos.y; 
    view_mat->m[2][2]= cam->target.z - cam_pos.z;

    magn= sqrtf(view_mat->m[2][0]* view_mat->m[2][0] 
             + view_mat->m[2][1]* view_mat->m[2][1] 
             + view_mat->m[2][2]* view_mat->m[2][2]);

    if (magn> 0.001f){
      magn= 1.0f/magn;
      view_mat->m[2][0]*= magn;
      view_mat->m[2][1]*= magn;
      view_mat->m[2][2]*= magn;
    }

    //RIGHT(TU*F),R1
    view_mat->m[0][0]= global.y * view_mat->m[2][2] - global.z * view_mat->m[2][1];
    view_mat->m[0][1]= -global.x * view_mat->m[2][2] + global.z * view_mat->m[2][0]; 
    view_mat->m[0][2]= global.x * view_mat->m[2][1] - global.y * view_mat->m[2][0];

    magn= sqrtf(view_mat->m[0][0]* view_mat->m[0][0] 
             + view_mat->m[0][1]* view_mat->m[0][1] 
             + view_mat->m[0][2]* view_mat->m[0][2]);

    if (magn> 0.001f){
      magn= 1.0f/magn;
      view_mat->m[0][0]*= magn;
      view_mat->m[0][1]*= magn;
      view_mat->m[0][2]*= magn;
    }

    //UP(F*R),R2
    view_mat->m[1][0]= view_mat->m[0][2] * view_mat->m[2][1] - view_mat->m[0][1] * view_mat->m[2][2]; 
    view_mat->m[1][1]= -view_mat->m[0][2] * view_mat->m[2][0] + view_mat->m[0][0] * view_mat->m[2][2];
    view_mat->m[1][2]= view_mat->m[0][1] * view_mat->m[2][0] - view_mat->m[0][0] * view_mat->m[2][1];

    view_mat->m[0][3]= -(view_mat->m[0][0]* cam_pos.x 
                     + view_mat->m[0][1]* cam_pos.y 
                     + view_mat->m[0][2]* cam_pos.z);

    view_mat->m[1][3]= -(view_mat->m[1][0]* cam_pos.x 
                     + view_mat->m[1][1]* cam_pos.y 
                     + view_mat->m[1][2]* cam_pos.z);

    view_mat->m[2][3]= -(view_mat->m[2][0]* cam_pos.x 
                     + view_mat->m[2][1]* cam_pos.y 
                     + view_mat->m[2][2]* cam_pos.z);

    //R4
    view_mat->m[3][0]= 0;
    view_mat->m[3][1]= 0;
    view_mat->m[3][2]= 0;
    view_mat->m[3][3]= 1;

    //printf("VIEWW\n");mat_print(view_mat);
    return;
}

//No w use yet
void build_ProjectionMatrix(
        EngineContext* engine,
        Scene* scene){

    Camera* cam= &scene->camera;

    mat4 proj_mat={0};
    float oot= 1.0f/tanf(cam->yFov/2.0f);
    float aspect= (float)engine->width/engine->height;

    proj_mat.m[0][0]= oot* 1.0f/aspect;
    proj_mat.m[1][1]= oot;
    proj_mat.m[2][2]= 1.0f;

    proj_mat.m[3][2]= 1.0f;

    cam->projection= proj_mat;

    //printf("PROJJ\n");mat_print(&proj_mat);
    return;
}

void build_ProjectionViewMatrix(Scene* scene){

    mat_mult_mat(
            &scene->camera.projection, 
            &scene->camera.view, &scene->ProjectionView);
    return;
}
