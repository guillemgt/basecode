#include "basecode.hpp"
#include "opengl.hpp"
#include "fonts.hpp"
#include "sound.hpp"
#include "ui.hpp"
#include "memory.hpp"

#include "opengl.cpp"
#include "fonts.cpp"
#include "sound.cpp"
#include "ui.cpp"
#include "memory.cpp"
#include "structures.cpp"
#include "models.cpp"

#if USE_SDL
#include "sdl2.cpp"
#endif

float TIME_STEP;
char RUN_TREE_PATH[MAX_PATH_LENGTH] = __FILE__;

void init_basecode(){
    // Compute RUN_TREE_PATH
    int l = 0;
    do{
        strrchr(RUN_TREE_PATH, '/')[0] = '\0';
        l = (int)strlen(RUN_TREE_PATH);
    }while(l > 0 && (RUN_TREE_PATH[l-1] != 'e' || RUN_TREE_PATH[l-2] != 'd' || RUN_TREE_PATH[l-3] != 'o' || RUN_TREE_PATH[l-4] != 'c' || RUN_TREE_PATH[l-5] != '/'));
    strrchr(RUN_TREE_PATH, '/')[0] = '\0';
    sprintf(RUN_TREE_PATH, "%s/run_tree/", RUN_TREE_PATH);
    
    init_basecode_memory();
#if ENGINE_ENABLE_OPENGL
    init_basecode_openGL();
#endif
#if ENGINE_ENABLE_FREETYPE
    init_fonts();
#endif
#if ENGINE_ENABLE_OPENAL
    init_basecode_sound();
#endif
#if ENGINE_ENABLE_UITHING
    initUIThing();
#endif
}

void cleanup_basecode(){
#if ENGINE_ENABLE_UITHING
    cleanup_ui_thing();
#endif
#if ENGINE_ENABLE_OPENAL
    cleanup_basecode_sound();
#endif
#if ENGINE_ENABLE_FREETYPE
    cleanup_fonts();
#endif
#if ENGINE_ENABLE_OPENGL
    cleanup_basecode_openGL();
#endif
    cleanup_basecode_memory();
}
