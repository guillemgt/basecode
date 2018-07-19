#ifndef engine_opengl_h
#define engine_opengl_h

#define OPENGL_OLD 1

#if OS == OS_WINDOWS

#define OPENGL_ES 0
#include <Windows.h>
#include <GL/glew.h>
#include <GL/gl.h>

#elif OS == OS_IOS

#define OPENGL_ES 1
#include <OpenGLES/EAGL.h>
#include <OpenGLES/ES3/gl.h>

#elif OS == OS_WASM

#define OPENGL_ES 1
#include <GL/glew.h>
//#include <GLES2/gl2.h>
#include <SDL/SDL.h> // ??

#else

#define OPENGL_ES 0
#include <OpenGL/gl.h>
#if !OPENGL_OLD
#include <OpenGL/gl3.h>
#endif

#endif

#if OS == OS_MAC && OPENGL_OLD
#define glGenVertexArrays    glGenVertexArraysAPPLE
#define glBindVertexArray    glBindVertexArrayAPPLE
#define glDeleteVertexArrays glDeleteVertexArraysAPPLE
#endif

#define check_openGL_error() _check_openGL_error(__LINE__, __FILE__)

struct GlBuffer{
    GLuint positions, colors, coords;
    unsigned int verts;
};

inline void initPositionsColors(GlBuffer *t){
    glGenBuffers(1, &t->positions);
    glGenBuffers(1, &t->colors);
    t->verts = 0;
}inline void cleanupPositionsColors(GlBuffer *t){
    glDeleteBuffers(1, &t->positions);
    glDeleteBuffers(1, &t->colors);
    t->verts = 0;
}

inline void initPositionsCoords(GlBuffer *t){
    glGenBuffers(1, &t->positions);
    glGenBuffers(1, &t->coords);
    t->verts = 0;
}
inline void cleanupPositionsCoords(GlBuffer *t){
    glDeleteBuffers(1, &t->positions);
    glDeleteBuffers(1, &t->coords);
    t->verts = 0;
}

inline void initPositionsColorsCoords(GlBuffer *t){
    glGenBuffers(1, &t->positions);
    glGenBuffers(1, &t->colors);
    glGenBuffers(1, &t->coords);
    t->verts = 0;
}
inline void cleanupPositionsColorsCoords(GlBuffer *t){
    glDeleteBuffers(1, &t->positions);
    glDeleteBuffers(1, &t->colors);
    glDeleteBuffers(1, &t->coords);
    t->verts = 0;
}

bool init_basecode_openGL();
bool cleanup_basecode_openGL();

struct Vertex_PC {
    Vec3 p;
    RgbColor c;
};
struct Vertex_PCa {
    Vec3 p;
    RgbaColor c;
};
struct Vertex_PTCa {
    Vec3 p;
    Vec2 t;
    RgbaColor c;
};
struct Vertex_PT {
    Vec3 p;
    Vec2 t;
};
struct Vertex_PTa {
    Vec3 p;
    Vec3 t;
};
struct Vertex_PNC {
    Vec3 p, n;
    RgbColor c;
};

template<typename T> inline void set_buffer_data(GLuint buffer, T *vert, u32 size, GLuint mode){
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, size*sizeof(T), vert, mode);
}
#define set_buffer_data_static(b, v, s) set_buffer_data(b, v, s, GL_STATIC_DRAW)
#define set_buffer_data_dynamic(b, v, s) set_buffer_data(b, v, s, GL_DYNAMIC_DRAW)
#define set_buffer_data_stream(b, v, s) set_buffer_data(b, v, s, GL_STREAM_DRAW)

void _check_openGL_error(int line, const char *file);

GLuint load_shaders(const char *file_path);
GLuint load_shaders(const char *file_path, const char *prefix);
GLuint load_shaders_by_text(const char *text);

void load_texture(GLuint *texture, const char *path);

extern Vec2i window_size;

#endif /* defined(__topo__render__) */
