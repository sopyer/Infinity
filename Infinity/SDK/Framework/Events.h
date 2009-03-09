#ifndef __EVENTS_H_INCLUDED__
#	define __EVENTS_H_INCLUDED__

#include <KeySyms.h>

class InputState
{
public:
	enum
	{
		Released = 0,
		Pressed  = 1
	};
};

class Modifiers
{
public:
	enum
	{
		Ctrl  = 1<<0,
		Alt   = 1<<1,
		Shift = 1<<2
	};
};

class MouseButtons
{
public:
	enum
	{
		Left   = 0,
		Right  = 1,
		Middle = 2,
		Aux0   = 3,
		Aux1   = 4,
		Aux2   = 5,
		Aux3   = 6,
		Aux4   = 7,
		Count  = 8
	};
};

struct ButtonEvent
{
	uint32	button;
	uint32	mModifiers;
	float	x, y;
};

struct MotionEvent
{
	uint32	mModifiers;
	float	x, y;
};

struct KeyEvent
{
	uint32	key;//Value;
	uint32	modifiers;
	//uint32	hwKeycode;
	uint32	unicode;//Char;
};

struct ScrollEvent
{
	uint32	deltaVert, deltaHorz;
	uint32	mModifiers;
	float	x, y;
};

#endif