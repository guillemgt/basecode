#ifndef engine_window_h
#define engine_window_h

#include "basecode.hpp"

enum {
    KEYS_UP    =  0,
    KEYS_RIGHT =  1,
    KEYS_DOWN  =  2,
    KEYS_LEFT  =  3,
    KEYS_SPACE =  4,
    KEYS_ESC   =  5,
    KEYS_SHIFT =  6,
    KEYS_ENTER =  7,
#if BASECODE_NO_LETTER_KEYS
#define KEYS_NUM 8
#else
    KEYS_A     =  8,
    KEYS_B     =  9,
    KEYS_C     = 10,
    KEYS_D     = 11,
    KEYS_E     = 12,
    KEYS_F     = 13,
    KEYS_G     = 14,
    KEYS_H     = 15,
    KEYS_I     = 16,
    KEYS_J     = 17,
    KEYS_K     = 18,
    KEYS_L     = 19,
    KEYS_M     = 20,
    KEYS_N     = 21,
    KEYS_O     = 22,
    KEYS_P     = 23,
    KEYS_Q     = 24,
    KEYS_R     = 25,
    KEYS_S     = 26,
    KEYS_T     = 27,
    KEYS_U     = 28,
    KEYS_V     = 29,
    KEYS_W     = 30,
    KEYS_X     = 31,
    KEYS_Y     = 32,
    KEYS_Z     = 33,
#define KEYS_NUM 34
#endif
};

#define MAX_EVENTS_PER_LOOP  256
#define EVENT_KEYDOWN        0
#define EVENT_KEYUP          1
#define EVENT_LEFTMOUSEDOWN  2
#define EVENT_LEFTMOUSEMOVE  3
#define EVENT_LEFTMOUSEUP    4
#define EVENT_RIGHTMOUSEDOWN 5
#define EVENT_RIGHTMOUSEMOVE 6
#define EVENT_RIGHTMOUSEUP   7
#define EVENT_RESIZE         8
#define EVENT_CLOSE          9
#define EVENT_FOCUS_LOST     10

struct MouseMovement{
    Vec2i r, v;
};

union EventData {
    char key;
    Vec2i size;
    Vec2i coords;
    MouseMovement move;
    inline EventData(){ // It needs this initializer somewhere...
    }
    inline EventData(char k) : key(k) {}
	inline EventData(int x, int y){
		size = { x, y };
	}
	inline EventData(int x, int y, int z, int w){
		move.r = { x, y };
		move.v = { z, w };
	}
};

struct Event{
    uint8 id;
    EventData data;
};

int init_game();
int game_loop(bool keys[KEYS_NUM], StaticArray<Event, MAX_EVENTS_PER_LOOP> events);
void game_draw();
void cleanup_game();



#endif /* main_hpp */
