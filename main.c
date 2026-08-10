#include "SDL3/SDL.h" 
#include "math.h"
#include "unistd.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "include/obj.h"
#include "include/main.h"
#include "include/matrix.h"
#include "include/rasterizer.h"

SDL_Window* window= NULL;
SDL_Renderer* renderer= NULL;
SDL_Texture* texture= NULL;
SDL_PixelFormat pixel_format= SDL_PIXELFORMAT_RGBA8888;

mat4 projview;

uint32_t pixels[WIDTH*HEIGHT];
float z_buffer[WIDTH*HEIGHT];
vec3 light= {0.0f,0.0f,1.0f};

void calc_meshes_norm(mesh* all_meshes,int mesh_count);
int project(object* object,uint32_t color);

void clean_buffer();

int main(){

    create_window();
    init_renderer();
    init_texture();

    camera* cam;
    cam->cam_pos.x= 0; cam->cam_pos.y= 0; cam->cam_pos.z= 0;
    cam->target.x= 0; cam->target.y= 0; cam->target.z= 100;
    cam->global.x= 0; cam->global.y= 1; cam->global.z= 0;
    create_view_lookat_mat(cam);
    create_proj_mat(cam);
    mat_mult_mat(&cam->projection,&cam->view,&projview);
    
    //TEST VALUES
    //1mesh,2faces,6vertices
    mesh* all_meshes;
    all_meshes= calloc(1,sizeof(mesh));
    mesh* my_mesh= &all_meshes[0];
    my_mesh->face_count= 1;
    my_mesh->faces= calloc(my_mesh->face_count,sizeof(face));
    my_mesh->vx_count= 3;
    my_mesh->vx_all= calloc(my_mesh->vx_count,sizeof(vertex));

    //anti-clockwise means front,vice-versa
    vertex* all_vx= my_mesh->vx_all;
    all_vx[0].pos.x= -3; all_vx[0].pos.y= 3; all_vx[0].pos.z= 6;             
    all_vx[1].pos.x= -3; all_vx[1].pos.y= -3; all_vx[1].pos.z= 1;
    all_vx[2].pos.x= 3; all_vx[2].pos.y= -3; all_vx[2].pos.z= 6;
    /*all_vx[3].pos.x= 3; all_vx[3].pos.y= 3; all_vx[3].pos.z= 5; 
    all_vx[4].pos.x= 0; all_vx[4].pos.y= 3.5; all_vx[4].pos.z= 6;
    all_vx[5].pos.x= -4; all_vx[5].pos.y= 2; all_vx[5].pos.z= 4;*/

    my_mesh->faces[0].vx_index[0]= 0; my_mesh->faces[0].vx_index[1]= 1; my_mesh->faces[0].vx_index[2]= 2;
    my_mesh->faces[1].vx_index[0]= 0; my_mesh->faces[1].vx_index[1]= 2; my_mesh->faces[1].vx_index[2]= 3;

    //1staticobject
    object* all_objs;
    all_objs= calloc(1,sizeof(object));
    object* my_obj= &all_objs[0];
    my_obj->mesh_data= my_mesh;
    my_obj->vx_trans= calloc(my_mesh->vx_count,sizeof(vertex));
    my_obj->rot.y= 0.0f;
    my_obj->pos.x= 0.0f; my_obj->pos.y= 0.0f; my_obj->pos.z= 4;

    memset(z_buffer,0,sizeof(z_buffer));
    clean_buffer();
    uint32_t fillcolor= 0xffffffff;

    calc_meshes_norm(all_meshes,1);
    bool isRunning= true;
    SDL_Event event;

    while (isRunning) {
        while (SDL_PollEvent(&event)){
            if ((&event)->type== SDL_EVENT_QUIT) {
                isRunning= false;
            }
        }
        clean_buffer();
        memset(z_buffer,0,sizeof(z_buffer));

        /*cam->target.y+=0.1f;
        create_view_lookat_mat(cam);
        projview= mat_mult_mat(proj,&cam->view);*/

        project(my_obj,fillcolor);

        SDL_UpdateTexture(texture,NULL,pixels,WIDTH*4);         //cpu to pcie lanes to vram

        SDL_RenderTexture(renderer,texture,NULL,NULL);          //draws into buffer

        SDL_RenderPresent(renderer);            //swap gpu buffers

        sleep(2);break;
        
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


bool render_StaticObjectFlat(Object* obj,vec3 lightDir){

    Mesh* mesh= obj->meshData;
    //if dynamic obj then vtrans calloc
    VertexOutput* vTrans= obj->verticesTransformed;

    //setMeshVerticesNormal(mesh);

    mat4 PVM;
    build_ModelMatrix(obj);
    mat_mult_mat(&projview,&obj->model,&PVM); 

    //Vertex shader
    FlatShader* shader;
    shader->lightDir= lightDir;
    shader->M= &obj->model;  //could be MV
    shader->PVM= &PVM;
    vertex_FlatShader(mesh->vertices,vTrans,shader,mesh->vertexCount);

    int num= mesh->faceCount;
    Face* faces= mesh->faces;
    VertexOutput* faceVertices= malloc(sizeof(VertexOutput)*3);
    for (int i= 0;i< num;i++){

        faceVertices[0]= vTrans[faces[i].verticesIndex[0]];
        faceVertices[1]= vTrans[faces[i].verticesIndex[1]];
        faceVertices[2]= vTrans[faces[i].verticesIndex[2]];

        vec3 v1= vTrans[faces[i].verticesIndex[0]].world_pos;
        vec3 v2= vTrans[faces[i].verticesIndex[1]].world_pos;
        vec3 v3= vTrans[faces[i].verticesIndex[2]].world_pos;

        vec3 e1= vec3Sub(v2,v1);
        vec3 e2= vec3Sub(v3,v1);

        shader->faceNorm= vec3Normalize(vec3Cross(e1,e2));
        shader->i= vec3Dot(shader->faceNorm,shader->lightDir);

        draw_triangles_barycentric(faceVertices,shader,SHADE_FLAT);
    }
}

bool render_StaticObjectGouraud(Object* obj){

    Mesh* mesh= obj->meshData;
    //if dynamic obj then vtrans calloc
    VertexOutput* vTrans= obj->verticesTransformed;

    //setMeshVerticesNormal(mesh);

    mat4 PVM;
    build_ModelMatrix(obj);
    mat_mult_mat(&projview,&obj->model,&PVM); 

    //Vertex shader
    GouraudShader* shader;
    shader->M= &obj->model;
    shader->PVM= &PVM;
    //TODO: normal matrix
    vertex_GouraudShader(mesh->vertices,vTrans,shader,mesh->vertexCount);

    int num= mesh->faceCount;
    Face* faces= mesh->faces;
    VertexOutput* faceVertices= malloc(sizeof(VertexOutput)*3);
    for (int i= 0;i< num;i++){

        faceVertices[0]= vTrans[faces[i].verticesIndex[0]];
        faceVertices[1]= vTrans[faces[i].verticesIndex[1]];
        faceVertices[2]= vTrans[faces[i].verticesIndex[2]];

        vec3 n1= vec3Normalize(faceVertices[0].world_pos);
        vec3 n2= vec3Normalize(faceVertices[1].world_pos);
        vec3 n3= vec3Normalize(faceVertices[2].world_pos);

        shader->i1= vec3Dot(n1,shader->lightDir);
        shader->i2= vec3Dot(n2,shader->lightDir);
        shader->i3= vec3Dot(n3,shader->lightDir);

        draw_triangles_barycentric(faceVertices,shader,SHADE_GOURAUD);
    }

    printf("=============\n");
    //scanline_rasterize(pnts,face_color[i+1],face_color[i+2]);
}

void clean_buffer(){
    uint32_t bg= 0x000000ff;
    for(int i=0;i<WIDTH*HEIGHT;i++){
        pixels[i]= bg;
    }
}

void create_window(){
    window= SDL_CreateWindow("hello-window",WIDTH,HEIGHT,0);
    if (window== NULL) {
        printf("Window_Error: %s\n", SDL_GetError());
        SDL_Quit();
    }
    return;
}

void init_renderer(){
    renderer= SDL_CreateRenderer(window, NULL);
    if (renderer== NULL) {
        printf("Renderer_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    return;
}

void init_texture(){
    texture= SDL_CreateTexture(renderer,pixel_format,SDL_TEXTUREACCESS_STREAMING,WIDTH,HEIGHT);
    if(texture== NULL){
        printf("texture_error %s",SDL_GetError());
        SDL_DestroyWindow(window); 
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
    }
    return;
}