#include "engine.hpp"
#include "opengl.hpp"
#include "freetype.hpp"
#include "sound.hpp"
#include "ui.hpp"
#include "memory.hpp"

#include "opengl.cpp"
#include "freetype.cpp"
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

void initEngine(){
    // Compute RUN_TREE_PATH
    int l = 0;
    do{
        strrchr(RUN_TREE_PATH, '/')[0] = '\0';
        l = (int)strlen(RUN_TREE_PATH);
    }while(l > 0 && (RUN_TREE_PATH[l-1] != 'e' || RUN_TREE_PATH[l-2] != 'd' || RUN_TREE_PATH[l-3] != 'o' || RUN_TREE_PATH[l-4] != 'c'));
    strrchr(RUN_TREE_PATH, '/')[0] = '\0';
    sprintf(RUN_TREE_PATH, "%s/run_tree/", RUN_TREE_PATH);
    
    initEngineMemory();
#if ENGINE_ENABLE_OPENGL
    initEngineOpenGL();
#endif
#if ENGINE_ENABLE_FREETYPE
    initFreetype();
#endif
#if ENGINE_ENABLE_OPENAL
    initSound();
#endif
#if ENGINE_ENABLE_UITHING
    initUIThing();
#endif
}

void cleanupEngine(){
#if ENGINE_ENABLE_UITHING
    cleanupUIThing();
#endif
#if ENGINE_ENABLE_OPENAL
    cleanupSound();
#endif
#if ENGINE_ENABLE_FREETYPE
    cleanupFreetype();
#endif
#if ENGINE_ENABLE_OPENGL
    cleanupEngineOpenGL();
#endif
    cleanupEngineMemory();
}
