#include <string.h>
#include "../include/matrix.h"
#include "../include/obj.h"
#include "../include/buffer.h"

//TODO:Quarternions implement
//Using rot vec gives gimbal

//XYZ
void build_ModelMatrix(Object* obj) {

    mat4 model_mat;
    vec3 pos= obj->pos;
    vec3 scale= obj->scale;
    
    //Raw angles
    float ax= obj->rot.x;
    float ay= obj->rot.y;
    float az= obj->rot.z;
    
    float cx= cosf(ax); float sx= sinf(ax);
    float cy= cosf(ay); float sy= sinf(ay);
    float cz= cosf(az); float sz= sinf(az);

    //R1
    model_mat.m[0][0]= (cy * cz) * scale.x;
    model_mat.m[0][1]= (sx * sy * cz - cx * sz) * scale.y;
    model_mat.m[0][2]= (cx * sy * cz + sx * sz) * scale.z;
    model_mat.m[0][3]= pos.x;

    //R2
    model_mat.m[1][0]= (cy * sz) * scale.x;
    model_mat.m[1][1]= (sx * sy * sz + cx * cz) * scale.y;
    model_mat.m[1][2]= (cx * sy * sz - sx * cz) * scale.z;
    model_mat.m[1][3]= pos.y;

    //R3
    model_mat.m[2][0]= (-sy) * scale.x;
    model_mat.m[2][1]= (sx * cy) * scale.y;
    model_mat.m[2][2]= (cx * cy) * scale.z;
    model_mat.m[2][3]= pos.z;

    //R4
    model_mat.m[3][0]= 0.0f;
    model_mat.m[3][1]= 0.0f;
    model_mat.m[3][2]= 0.0f;
    model_mat.m[3][3]= 1.0f;

    // Save final matrix back to the object structure
    obj->model= model_mat;
}

void build_NormalMatrix(mat4* normMat,mat4* modelMat){

    float (*m)[4]= modelMat->m;

    float det;
    det= m[0][0]*(m[1][1] * m[2][2] - m[1][2] * m[2][1])
        -m[0][1]*(m[1][2] * m[2][0] - m[1][0] * m[2][2]) //sign    
        +m[0][2]*(m[1][0] * m[2][1] - m[1][1] * m[2][0]);

    //0 determinant 
    if (fabsf(det) < 0.0001f){
        memset(normMat, 0, sizeof(mat4));
        normMat->m[0][0] = normMat->m[1][1] = normMat->m[2][2] = normMat->m[3][3] = 1.0f;
        return;
    }

    det = 1.0f / det;
    // Row 1 of Normal Matrix 
    normMat->m[0][0]= (m[1][1] * m[2][2] - m[1][2] * m[2][1]) * det;
    normMat->m[0][1]= (m[1][2] * m[2][0] - m[1][0] * m[2][2]) * det;
    normMat->m[0][2]= (m[1][0] * m[2][1] - m[1][1] * m[2][0]) * det;
    normMat->m[0][3]= 0;

    // Row 2 of Normal Matrix 
    normMat->m[1][0]= (m[0][2] * m[2][1] - m[0][1] * m[2][2]) * det; 
    normMat->m[1][1]= (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * det;
    normMat->m[1][2]= (m[0][1] * m[2][0] - m[0][0] * m[2][1]) * det; 
    normMat->m[1][3]= 0;

    // Row 3 of Normal Matrix 
    normMat->m[2][0]= (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * det;
    normMat->m[2][1]= (m[0][2] * m[1][0] - m[0][0] * m[1][2]) * det; 
    normMat->m[2][2]= (m[0][0] * m[1][1] - m[0][1] * m[1][0]) * det;
    normMat->m[2][3]= 0;

    // Row 4 of Normal Matrix 
    normMat->m[3][0]= 0;
    normMat->m[3][1]= 0;
    normMat->m[3][2]= 0;
    normMat->m[3][3]= 0;
}

/*LOOKAT matrix wont work if camera is pointing straight up or down
For that its better to switch to global_z or Quaternions*/
void build_ViewLookAtMatrix(Camera* cam){

   mat4 view_mat;
   vec3 cam_pos= cam->cam_pos;
   vec3 global= cam->global;
   float magn;

   //FORWARD,R3
   view_mat.m[2][0]= cam->target.x - cam_pos.x; 
   view_mat.m[2][1]= cam->target.y - cam_pos.y; 
   view_mat.m[2][2]= cam->target.z - cam_pos.z;

   magn= sqrtf(view_mat.m[2][0]* view_mat.m[2][0] 
             + view_mat.m[2][1]* view_mat.m[2][1] 
	         + view_mat.m[2][2]* view_mat.m[2][2]);

   if (magn> 0.0001f){
      magn= 1.0f/magn;
      view_mat.m[2][0]*= magn;
      view_mat.m[2][1]*= magn;
      view_mat.m[2][2]*= magn;
   }

   //RIGHT(U*F),R1
   view_mat.m[0][0]= global.y* view_mat.m[2][2] - global.z* view_mat.m[2][1];
   view_mat.m[0][1]= global.z* view_mat.m[2][0] - global.x* view_mat.m[2][2];
   view_mat.m[0][2]= global.x* view_mat.m[2][1] - global.y* view_mat.m[2][0];

   magn= sqrtf(view_mat.m[0][0]* view_mat.m[0][0] 
	         + view_mat.m[0][1]* view_mat.m[0][1] 
	         + view_mat.m[0][2]* view_mat.m[0][2]);

   if (magn> 0.0001f){
      magn= 1.0f/magn;
      view_mat.m[0][0]*= magn;
      view_mat.m[0][1]*= magn;
      view_mat.m[0][2]*= magn;
   }

   //UP(F*R),R2
   view_mat.m[1][0]= view_mat.m[2][1]* view_mat.m[0][2] - view_mat.m[2][2]* view_mat.m[0][1]; 
   view_mat.m[1][1]= view_mat.m[2][2]* view_mat.m[0][0] - view_mat.m[2][0]* view_mat.m[0][2]; 
   view_mat.m[1][2]= view_mat.m[2][0]* view_mat.m[0][1] - view_mat.m[2][1]* view_mat.m[0][0];

   view_mat.m[0][3]= -(view_mat.m[0][0]* cam_pos.x 
	                 + view_mat.m[0][1]* cam_pos.y 
		             + view_mat.m[0][2]* cam_pos.z);

   view_mat.m[1][3]= -(view_mat.m[1][0]* cam_pos.x 
		             + view_mat.m[1][1]* cam_pos.y 
		             + view_mat.m[1][2]* cam_pos.z);

   view_mat.m[2][3]= -(view_mat.m[2][0]* cam_pos.x 
		             + view_mat.m[2][1]* cam_pos.y 
		             + view_mat.m[2][2]* cam_pos.z);

   //R4
   view_mat.m[3][0]= 0;
   view_mat.m[3][1]= 0;
   view_mat.m[3][2]= 0;
   view_mat.m[3][3]= 1;

   cam->view= view_mat;
   //printf("VIEWW\n");mat_print(&view_mat);
   return;
}

//no w use yet but still have it
void build_ProjectionMatrix(Camera* cam,Framebuffer* fb){

   mat4 projMat={0};
   float oot= 1.0f/tanf(cam->yFov/2.0f);
   float aspect= (float)fb->width/fb->height;

   projMat.m[0][0]= oot* 1.0f/aspect;
   projMat.m[1][1]= oot;
   projMat.m[2][2]= 1.0f;

   projMat.m[3][2]= 1.0f;

   cam->projection= projMat;
   //printf("PROJJ\n");mat_print(projMat);
   return;
}
