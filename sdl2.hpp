#ifndef engine_sdl2_h
#define engine_sdl2_h

#include <stdio.h>
#include "basecode.hpp"

#if OS != OS_IOS
#if OS != OS_WASM
#include <SDL2/SDL.h>
#else
#include <SDL/SDL.h>
#endif
#endif

#include "window.hpp"

#endif /* sdl2_hpp */
