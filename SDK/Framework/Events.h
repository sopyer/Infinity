#ifndef __EVENTS_H_INCLUDED__
#	define __EVENTS_H_INCLUDED__

#include <SDL2/SDL.h>

struct SDL_MouseButtonEvent;
struct SDL_MouseMotionEvent;
struct SDL_KeyboardEvent;
struct SDL_MouseButtonEvent;

#define ButtonEvent	SDL_MouseButtonEvent
#define MotionEvent	SDL_MouseMotionEvent
#define KeyEvent	SDL_KeyboardEvent
#define ScrollEvent	SDL_MouseButtonEvent

#endif