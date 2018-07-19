#ifndef engine_models_h
#define engine_models_h

#define MAX_MODEL_TRIANGLES 104
StaticArray<Vertex_PNC[3], MAX_MODEL_TRIANGLES> wall_model;

typedef StaticArray<Vertex_PNC, 3*MAX_MODEL_TRIANGLES> StaticModel;

StaticModel *get_static_model(const char *path, StaticModel *static_model);
StaticModel *get_static_model_absolute(const char *path, StaticModel *static_model);

#endif /* defined(__ta__models__) */
