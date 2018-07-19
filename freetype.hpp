#ifndef engine_freetype_h
#define engine_freetype_h

#include "opengl.hpp"

#define FREETYPE_SAMPLER 2

int initFreetype();
int cleanupFreetype();
int load_font(const char *path);

#ifdef TA_DEVMODE
void _cleanScreenLog();
void _screenLog(const char *ftext, ...);
#define cleanScreenLog(...) _cleanScreenLog(__VA_ARGS__)
#define print(...) _screenLog(__VA_ARGS__)
#else
#define cleanScreenLog(...)
#define print(...)
#endif

typedef enum {
    TEXT_ALIGN_TOP,
    TEXT_ALIGN_BOTTOM,
} TextAlignment;

void renderText(float x, float y, float size, char *printLog);
unsigned int renderText(float x, float y, float z, uint8 quality, String &s, Vec3 *pos, RgbaColor *col, Vec2* coords, float font_size, float *width, float *height);
unsigned int renderText(float x, float y, float z, uint8 quality, String &s, Vec3 *pos, RgbaColor *col, Vec2* coords, float font_size, float *width, float *height, RgbaColor color);
u32 render_text(float x, float y, float z, u8 quality, String &s, Vertex_PTCa *vertices, float font_size, float *width, float *height, RgbaColor color, TextAlignment align);
u32 render_text_monospace(float x, float y, float z, u8 quality, String &s, Vertex_PTCa *vertices, float font_size, float *width, float *height, RgbaColor color, TextAlignment align);
float renderTextLength(uint8 size, String &s);
float renderTextLength(uint8 size, String &s, int length);
int textVertNum(char *printLog);
int textVertNum(String &s);

#define FONT_QUALITIES 4
#define FONT_QUALITY_12 0
#define FONT_QUALITY_16 1
#define FONT_QUALITY_32 2
#define FONT_QUALITY_64 3

#endif /* defined(__ta__files__) */

