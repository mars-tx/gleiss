#include <stdio.h>
#include "include/obj.h"
#include "include/main.h"
#include  "include/vector.h"

void mat_print(mat4* mat){
   for(int i= 0;i< 4;i++){
      for(int j= 0;j< 4;j++){
         printf("%f ",mat->m[i][j]);
      }
      printf("\n");
   }
}

//TODO:Quarternions implement
//Using rot vec gives gimbal
void buildModelMatrix(Object* obj){

   mat4 model_mat={0};
   vec3 pos= obj->pos;
   vec3 scale= obj->scale;
   float b= obj->rot.y;
   float cosb= cosf(b);
   float sinb= sinf(b);

   model_mat.m[0][0]= cosb*scale.x; model_mat.m[0][2]= -sinb; model_mat.m[0][3]= pos.x;

   model_mat.m[1][1]= scale.y; model_mat.m[1][3]= pos.y; 

   model_mat.m[2][0]= -sinb; model_mat.m[1][2]= cosb*scale.z; model_mat.m[2][3]= pos.z;

   model_mat.m[3][3]= 1;

   obj->model= model_mat;
   return;
}

/*LOOKAT matrix wont work if camera is pointing straight up or down
For that its better to switch to global_z or Quaternions*/

void buildViewLookAtMatrix(Camera* cam){

   mat4 view_mat={0};
   vec3 cam_pos= cam->cam_pos;
   vec3 global= cam->global;
   float magn;
   //FORWARD
   view_mat.m[2][0]= cam->target.x- cam_pos.x; view_mat.m[2][1]= cam->target.y- cam_pos.y; view_mat.m[2][2]= cam->target.z- cam_pos.z;
   magn= sqrtf(view_mat.m[2][0]*view_mat.m[2][0] + view_mat.m[2][1]*view_mat.m[2][1] + view_mat.m[2][2]*view_mat.m[2][2]);
   if (magn> 0.0001f){
      magn= 1.0f/magn;
      view_mat.m[2][0]*= magn;
      view_mat.m[2][1]*= magn;
      view_mat.m[2][2]*= magn;
   }

   //RIGHT(U*F)
   view_mat.m[0][0]= global.y* view_mat.m[2][2] - global.z* view_mat.m[2][1];
   view_mat.m[0][1]= global.z* view_mat.m[2][0] - global.x* view_mat.m[2][2];
   view_mat.m[0][2]= global.x* view_mat.m[2][1] - global.y* view_mat.m[2][0];
   magn= sqrtf(view_mat.m[0][0]*view_mat.m[0][0] + view_mat.m[0][1]*view_mat.m[0][1] + view_mat.m[0][2]*view_mat.m[0][2]);
   if (magn> 0.0001f){
      magn= 1.0f/magn;
      view_mat.m[0][0]*= magn;
      view_mat.m[0][1]*= magn;
      view_mat.m[0][2]*= magn;
   }

   //UP(F*R)
   view_mat.m[1][0]= view_mat.m[2][1]* view_mat.m[0][2] - view_mat.m[2][2]* view_mat.m[0][1]; 
   view_mat.m[1][1]= -view_mat.m[2][0]* view_mat.m[0][2] + view_mat.m[2][2]* view_mat.m[0][0]; 
   view_mat.m[1][2]= view_mat.m[2][0]* view_mat.m[0][1] - view_mat.m[2][1]* view_mat.m[0][0];

   view_mat.m[0][3]= -(view_mat.m[0][0]* cam_pos.x + view_mat.m[0][1]* cam_pos.y + view_mat.m[0][2]* cam_pos.z);
   view_mat.m[1][3]= -(view_mat.m[1][0]* cam_pos.x + view_mat.m[1][1]* cam_pos.y + view_mat.m[1][2]* cam_pos.z);
   view_mat.m[2][3]= -(view_mat.m[2][0]* cam_pos.x + view_mat.m[2][1]* cam_pos.y + view_mat.m[2][2]* cam_pos.z);

   view_mat.m[3][3]= 1;

   cam->view= view_mat;
   //printf("VIEWW\n");mat_print(&view_mat);
   return;
}

//no w use yet but still have it
void buildProjectionMatrix(Camera* cam){

   mat4 proj_mat={0};
   float oot= 1.0/tanf(FOV_ANGLE_Y/2.0f);
   float aspect= (float)WIDTH/HEIGHT;

   proj_mat.m[0][0]= oot* 1.0f/aspect;
   proj_mat.m[1][1]= oot;
   proj_mat.m[2][2]= 1.0f;

   proj_mat.m[3][2]= 1.0f;

   cam->projection= proj_mat;
   //printf("PROJJ\n");mat_print(proj_mat);
   return;
}

vec3 MatMultVec(mat4* matrix,vec3 vec){

   vec3 result={0};

   result.x= matrix->m[0][0]* vec.x + matrix->m[0][1]* vec.y + matrix->m[0][2]* vec.z + matrix->m[0][3];
   result.y= matrix->m[1][0]* vec.x + matrix->m[1][1]* vec.y + matrix->m[1][2]* vec.z + matrix->m[1][3];
   result.z= matrix->m[2][0]* vec.x + matrix->m[2][1]* vec.y + matrix->m[2][2]* vec.z + matrix->m[2][3];

   //printf("r %f %f %f v %f %f %f\n",result.x,result.y,result.z,vec->x,vec->y,vec->z);
   return result;
}

void MatMultMat(mat4* a,mat4* b,mat4* out){

   for (int r= 0;r< 4;r++){
      for(int c= 0;c< 4;c++){
         out->m[r][c]= 0;
         for(int k= 0;k< 4;k++){
            out->m[r][c]+= a->m[r][k] * b->m[k][c];
         }
      }
   }
   //printf("MULTT\n");mat_print(&out);
}