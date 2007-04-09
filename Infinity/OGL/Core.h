#pragma once

#include <windows.h>
#include <string>
#include <map>
#include "types.h"
#include "debug.h"
#include <gl\glew.h>
#include <gl\wglew.h>
#include "cursor.h"

/*-------------------------------------------------------------------------*/
/**
	@brief    This class handles render creation, input processing, event processing
**/
/*--------------------------------------------------------------------------*/
//! Interface of an object wich can receive events.
class IEventReceiver
{
public:

	virtual ~IEventReceiver() {};

	//! called if an event happened. returns true if event was processed
	virtual void OnResize(uint32 newWidth, uint32 newHeight) = 0;
	virtual void OnMouseUp(float x, float y, uint32 state) = 0;
	virtual void OnMouseDown(float x, float y, uint32 state) = 0;
	virtual void OnMouseMove(float x, float y) = 0;
	virtual void OnMouseWheel(float delta) = 0;
	virtual void OnKeyUp(uint32 key, uint32 funcState) = 0;
	virtual void OnKeyDown(uint32 key, uint32 funcState) = 0;
};

class Core
{
public:
	struct SCreateParams
	{
		int		mWidth;
		int		mHeight;
		int		mBitsPerPixel;
		int		mDepthBits;
		int		mStencilBits;
		bool	mFullscreen;
		bool	mVSync;
	};

public:
//!
	Core(){}
	~Core(){destroy();}
	
	bool create(SCreateParams &params);
	Cursor* getCursor(){return mCursor;}
	void setWindowCaption(std::string caption){SetWindowTextA(hWnd, caption.c_str());}
	void setEventReceiver(IEventReceiver* recv){mReceiver=recv;}
	static bool run();
	void destroy();
	void present(){SwapBuffers(hDC);}

private:
	bool switchToFullscreen(SCreateParams &params);

	bool createGLRender(SCreateParams &params);

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
	HWND	hWnd;
	HDC		hDC;
	HGLRC	hRC;
	Cursor	*mCursor;
	SCreateParams	mParams;
	IEventReceiver	*mReceiver;
	typedef std::map<HWND, Core*> EnvMap;

	static EnvMap mEnvMap;

//	static bool glewIsInitialized=false;
};