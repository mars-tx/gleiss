#include <stdio.h>
#include "../include/vector.h"

typedef struct{
    float m[4][4];

}mat4;

void mat_print(mat4* mat){
   for(int i= 0;i< 4;i++){
      for(int j= 0;j< 4;j++){
         printf("%f ",mat->m[i][j]);
      }
      printf("\n");
   }
}

vec3 mat_mult_vec(mat4* matrix,vec3 vec){

   vec3 result;

   result.x= matrix->m[0][0]*vec.x + matrix->m[0][1]*vec.y + matrix->m[0][2]*vec.z 
           + matrix->m[0][3];
   result.y= matrix->m[1][0]*vec.x + matrix->m[1][1]*vec.y + matrix->m[1][2]*vec.z 
           + matrix->m[1][3];
   result.z= matrix->m[2][0]*vec.x + matrix->m[2][1]*vec.y + matrix->m[2][2]*vec.z 
           + matrix->m[2][3];

   //printf("r %f %f %f v %f %f %f\n",result.x,result.y,result.z,vec->x,vec->y,vec->z);
   return result;
}

void mat_mult_mat(mat4* a,mat4* b,mat4* out){

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
