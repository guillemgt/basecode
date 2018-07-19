#include <stdint.h>
#include <thread>

#include "basecode.hpp"
#include "models.hpp"

StaticModel *get_static_model(const char *path, StaticModel *static_model){
    OsFile fp = open_game_file(path, "rb");
    if(fp == nullptr){
        printf("Couldn't read '%s'\n", path);
        return nullptr;
    }
    u32 size;
    read_file(&size, sizeof(u32), 1, fp);
    
    Vec3     positions[3*MAX_MODEL_TRIANGLES];
    Vec3     normals[MAX_MODEL_TRIANGLES];
    RgbColor colors[MAX_MODEL_TRIANGLES];
    read_file(positions, sizeof(Vec3), 3*size, fp);
    read_file(colors, sizeof(RgbColor), size, fp);
    read_file(normals, sizeof(Vec3), size, fp);
    close_file(fp);
    
    static_model->size = 3*size;
    for(int i=0, j=0; i<size; i++){
        static_model->data[j] = {positions[j], normals[i], colors[i]};
        j++;
        static_model->data[j] = {positions[j], normals[i], colors[i]};
        j++;
        static_model->data[j] = {positions[j], normals[i], colors[i]};
        j++;
    }
    
    return static_model;
}
    
/*StaticModel *get_static_model_absolute(const char *path, StaticModel *static_model){
    OsFile fp = openFile(path, "rb");
    if(fp == nullptr){
        printf("Couldn't read '%s'\n", path);
        return nullptr;
    }
    u32 size;
    read_file(&size, sizeof(u32), 1, fp);
    
    Vec3     positions[3*MAX_MODEL_TRIANGLES];
    Vec3     normals[MAX_MODEL_TRIANGLES];
    RgbColor colors[MAX_MODEL_TRIANGLES];
    read_file(positions, sizeof(Vec3), 3*size, fp);
    read_file(colors, sizeof(RgbColor), size, fp);
    read_file(normals, sizeof(Vec3), size, fp);
    closeFile(fp);
    
    static_model->size = 3*size;
    for(int i=0, j=0; i<size; i++){
        static_model->data[j] = {positions[j], normals[i], colors[i]};
        j++;
        static_model->data[j] = {positions[j], normals[i], colors[i]};
        j++;
        static_model->data[j] = {positions[j], normals[i], colors[i]};
        j++;
    }
    
    return static_model;
}*/
