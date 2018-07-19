#include <iostream>

#include "basecode.hpp"
#include "opengl.hpp"
#include "window.hpp"
#include "sdl2.hpp"

extern Vec2i window_size;
extern f32 TIME_STEP;

#if OS != OS_WASM
#define WindowThingy SDL_Window
#else
#include <emscripten.h>
#define WindowThingy SDL_Surface
#endif

bool InitSDL(WindowThingy **window, SDL_GLContext *context){
#if OS != OS_WASM
    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("Failed to init SDL\n");
        return false;
    }
    
    // Create our window centered at 512x512 resolution
    *window = SDL_CreateWindow("LAGAGLGLA", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 680, 680, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    window_size = Vec2i(680, 680);
    
    // Check that everything worked out okay
    if(!(*window)){
        printf("Unable to create window\n");
        //CheckSDLError(__LINE__);
        return false;
    }
#else
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0){
        printf("Failed to init SDL\n");
        return false;
    }
    *window = SDL_SetVideoMode(0, 0, 24, SDL_OPENGL | SDL_RESIZABLE);
    windowSize = Vec2i((*window)->w, (*window)->h);
    if ( !(*window) ) {
        printf("Unable to set video mode: %s\n", SDL_GetError());
        return 1;
    }
#endif
    
#if OPENGL_OLD
#if OS != OS_WASM
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
#endif
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 4);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 4);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 4);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, 4);
    
#if OS != OS_WASM
    *context = SDL_GL_CreateContext(*window);
#endif
    
    SDL_GL_SetSwapInterval(1); // vsync
    return true;
}

void quitSDL(SDL_Window *window, SDL_GLContext context){
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

const int wantedKeys[] = {SDL_SCANCODE_UP, SDL_SCANCODE_RIGHT, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_SPACE, SDL_SCANCODE_ESCAPE, SDL_SCANCODE_LSHIFT, SDL_SCANCODE_EXECUTE, SDL_SCANCODE_A, SDL_SCANCODE_B, SDL_SCANCODE_C, SDL_SCANCODE_D, SDL_SCANCODE_E, SDL_SCANCODE_F, SDL_SCANCODE_G, SDL_SCANCODE_H, SDL_SCANCODE_I, SDL_SCANCODE_J, SDL_SCANCODE_K, SDL_SCANCODE_L, SDL_SCANCODE_M, SDL_SCANCODE_N, SDL_SCANCODE_O, SDL_SCANCODE_P, SDL_SCANCODE_Q, SDL_SCANCODE_R, SDL_SCANCODE_S, SDL_SCANCODE_T, SDL_SCANCODE_U, SDL_SCANCODE_V, SDL_SCANCODE_W, SDL_SCANCODE_X, SDL_SCANCODE_Y, SDL_SCANCODE_Z};
bool keys[KEYS_NUM] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
StaticArray<Event, MAX_EVENTS_PER_LOOP> events = StaticArray<Event, MAX_EVENTS_PER_LOOP>();
WindowThingy *window;

inline void SDL_Loop(){
#if OS != OS_WASM
#define scancode(n) n
#else
#define scancode(n) SDL_SCANCODE_TO_KEYCODE(n)
#endif
    
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    for(int i=0; i<KEYS_NUM; i++){
        keys[i] = state[wantedKeys[i]];
    }
    
    free(events);
    SDL_Event e;
    while(SDL_PollEvent(&e)){
        switch(e.type){
            case SDL_KEYDOWN: {
                char c;
                switch(e.key.keysym.scancode){
                    case SDL_SCANCODE_BACKSPACE:
                        c = 0;
                        break;
                    case SDL_SCANCODE_UP:
                        c = 1;
                        break;
                    case SDL_SCANCODE_RIGHT:
                        c = 2;
                        break;
                    case SDL_SCANCODE_DOWN:
                        c = 3;
                        break;
                    case SDL_SCANCODE_LEFT:
                        c = 4;
                        break;
                    case SDL_SCANCODE_SPACE:
                    case SDL_SCANCODE_EXECUTE:
                        c = 5;
                        break;
                    case SDL_SCANCODE_ESCAPE:
                        break;
                    default:
                        c = 6;
                        break;
                }
                events.push({EVENT_KEYDOWN, EventData(c)});
            }   break;
            case SDL_KEYUP: {
                char c;
                switch(e.key.keysym.scancode){
                    case SDL_SCANCODE_BACKSPACE:
                        c = 0;
                        break;
                    case SDL_SCANCODE_UP:
                        c = 1;
                        break;
                    case SDL_SCANCODE_RIGHT:
                        c = 2;
                        break;
                    case SDL_SCANCODE_DOWN:
                        c = 3;
                        break;
                    case SDL_SCANCODE_LEFT:
                        c = 4;
                        break;
                    default:
                        c = 5;
                        break;
                }
                events.push({EVENT_KEYUP, EventData(c)});
            }   break;
            case SDL_MOUSEBUTTONDOWN:
                if(e.button.button == SDL_BUTTON_LEFT)
                    events.push({EVENT_LEFTMOUSEDOWN,  EventData(e.button.x, e.button.y)});
                else if(e.button.button == SDL_BUTTON_RIGHT)
                    events.push({EVENT_RIGHTMOUSEDOWN, EventData(e.button.x, e.button.y)});
                break;
            case SDL_MOUSEBUTTONUP:
                if(e.button.button == SDL_BUTTON_LEFT)
                    events.push({EVENT_LEFTMOUSEUP,  EventData(e.button.x, e.button.y)});
                else if(e.button.button == SDL_BUTTON_RIGHT)
                    events.push({EVENT_RIGHTMOUSEUP, EventData(e.button.x, e.button.y)});
                break;
            case SDL_MOUSEMOTION:
                //if(e.button.button == SDL_BUTTON_LEFT)
                events.push({EVENT_LEFTMOUSEMOVE,  EventData(e.button.x, e.button.y, e.motion.xrel, e.motion.yrel)});
                //if(e.button.button == SDL_BUTTON_RIGHT)
                //events.push({EVENT_RIGHTMOUSEMOVE, EventData(e.button.x, e.button.y, e.motion.xrel, -(float)e.motion.yrel)});
                break;
#if OS == OS_WASM
            case SDL_VIDEORESIZE:
                windowSize = Vec2i(e.resize.w, e.resize.h);
                events.push({EVENT_RESIZE, EventData(e.resize.w, e.resize.h)});
                break;
#endif
            case SDL_WINDOWEVENT:
                switch(e.window.event){
                    case SDL_WINDOWEVENT_RESIZED:
                        window_size = Vec2i(e.window.data1, e.window.data2);
                        events.push({EVENT_RESIZE, EventData(e.window.data1, e.window.data2)});
                        break;
                    case SDL_WINDOWEVENT_CLOSE:
                        events.push({EVENT_CLOSE, EventData('0')});
                        break;
                    case SDL_WINDOWEVENT_FOCUS_LOST:
                        events.push({EVENT_FOCUS_LOST, EventData('0') });
                        break;
                }
                break;
            default:
                break;
        }
    }

#if OS == OS_WASM
    if(!game_logic(keys, events)){
        emscripten_pause_main_loop();
    }
#endif
    
    game_draw();
    
#if OS != OS_WASM
    SDL_GL_SwapWindow(window);
#else
    SDL_GL_SwapBuffers();
#endif
}

int main(int argc, char ** argv){
    SDL_GLContext context;
    if(!InitSDL(&window, &context))
        return -1;
    
    //int majVersion, minVersion;
    //SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &majVersion);
    //SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minVersion);
    //printf("Got OpenGL %i.%i\n", majVersion, minVersion);
    
    
#ifdef _WIN32
    glewExperimental = GL_TRUE;
    glewInit();
#endif
    
    TIME_STEP = 1./60.;
    
    init_basecode();
    
    init_game();
    
    
#if OS != OS_WASM
    do{
        SDL_Loop();
    } while(game_loop(keys, events));
#else
    emscripten_set_main_loop(SDL_Loop, 0, 1);
#endif
}

#if OS == OS_WASM
#include "../render.hpp"

extern "C" {
    
    void jsResizeWindow(int x, int y){
        windowSize = Vec2i(x, y);
        change_window_size();
    }
    
}
#endif
