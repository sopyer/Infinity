#include "Core.h"

Core::EnvMap Core::mEnvMap;

class NullReceiver: public IEventReceiver
{
public:
	//! called if an event happened. returns true if event was processed
	virtual void OnResize(uint32 newWidth, uint32 newHeight);
	virtual void OnMouseUp(float x, float y, uint32 state);
	virtual void OnMouseDown(float x, float y, uint32 state);
	virtual void OnMouseMove(float x, float y);
	virtual void OnMouseWheel(float delta);
	virtual void OnKeyUp(uint32 key, uint32 funcState);
	virtual void OnKeyDown(uint32 key, uint32 funcState);
};

void NullReceiver::OnResize(uint32 newWidth, uint32 newHeight)
{
}
void NullReceiver::OnMouseUp(float x, float y, uint32 state)
{
}
void NullReceiver::OnMouseDown(float x, float y, uint32 state)
{
}
void NullReceiver::OnMouseMove(float x, float y)
{
	int a=1;
	a++;
}
void NullReceiver::OnMouseWheel(float delta)
{
}
void NullReceiver::OnKeyUp(uint32 key, uint32 funcState)
{
}
void NullReceiver::OnKeyDown(uint32 key, uint32 funcState)
{
}

bool Core::create(SCreateParams &params)
{
	static NullReceiver nullRecv;
	static const char* className = "... Engine class";
	HINSTANCE hInstance = GetModuleHandle(0);
	WNDCLASSEX WndClass = {	sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW | CS_OWNDC, 
						&WndProc, 0L, 0L, 
						hInstance, NULL/*hIcon*/, NULL/*hCursor*/, 
						(HBRUSH)GetStockObject(BLACK_BRUSH)/*hbrBackground*/, NULL,
						className, NULL };

	//Bug, check if class exist?????????
	if( !RegisterClassEx( &WndClass ) )
	{
		return false;
	}

	mParams = params;
	mReceiver = &nullRecv;

	RECT clientSize;
	clientSize.top = 0;
	clientSize.left = 0;
	clientSize.right = params.mWidth;
	clientSize.bottom = params.mHeight;
	
	DWORD style = WS_POPUP;

	if (!params.mFullscreen)
		style = WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	AdjustWindowRect(&clientSize, style, FALSE);

	int32 realWidth = clientSize.right - clientSize.left;
	int32 realHeight = clientSize.bottom - clientSize.top;

	int32 windowLeft = (GetSystemMetrics(SM_CXSCREEN) - realWidth) / 2;
	int32 windowTop = (GetSystemMetrics(SM_CYSCREEN) - realHeight) / 2;

	if (mParams.mFullscreen)
	{
		windowLeft = 0;
		windowTop = 0;
	}

	// create window

	hWnd = CreateWindow( className, "", style, windowLeft, windowTop, 
				realWidth, realHeight,	NULL, NULL, hInstance, NULL);

	mEnvMap[hWnd]=this;

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	mCursor = new Cursor(hWnd);
	// fix ugly ATI driver bugs. Thanks to ariaci??????????????????
	MoveWindow(hWnd, windowLeft, windowTop, realWidth, realHeight, TRUE);

	if(params.mFullscreen)
		switchToFullscreen(params);
	
	createGLRender(params);

	SetActiveWindow(hWnd);
	SetForegroundWindow(hWnd);
	
	return true;
}

void Core::destroy()
{
	if( mParams.mFullscreen )
		ChangeDisplaySettings(NULL, 0);
	if( hDC )
	{	
		wglMakeCurrent( hDC, 0 );	
		if( hRC )			
		{
			wglDeleteContext( hRC );	
			hRC = NULL;				
		}
		
		ReleaseDC( hWnd, hDC );		
		hDC = NULL;					
	}

	if( hWnd )
	{
		mEnvMap.erase(hWnd);
		DestroyWindow(hWnd);
		hWnd = NULL;
		if( mCursor )
			delete mCursor;
		mCursor = 0;
	}
}

bool Core::run()
{
	MSG msg;

	bool quit = false;
	const int maxProcessed = 1;
	int processed = 0;
	while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) && processed < maxProcessed)
	{
		GetMessage(&msg, NULL, 0, 0);
		TranslateMessage(&msg);

		//if (ExternalWindow && msg.hwnd == HWnd)
		//	WndProc(HWnd, msg.message, msg.wParam, msg.lParam);
		//else
		DispatchMessage(&msg);

		if (msg.message == WM_QUIT)
			quit = true;
		++processed;
	}

	return !quit;
}

bool Core::switchToFullscreen(SCreateParams &params)
{
	DEVMODE dm;
	memset(&dm, 0, sizeof(dm));
	dm.dmSize = sizeof(dm);
	dm.dmPelsWidth = params.mWidth;
	dm.dmPelsHeight	= params.mHeight;
	dm.dmBitsPerPel	= params.mBitsPerPixel;
	dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	LONG ret = ChangeDisplaySettings(&dm, CDS_FULLSCREEN);
	
	switch(ret)
	{
		case DISP_CHANGE_SUCCESSFUL:
			//ChangedToFullScreen = true;
			return true;
		case DISP_CHANGE_RESTART:
			debug("Switch to fullscreen: The computer must be restarted in order for the graphics mode to work.\n");
			return false;
		case DISP_CHANGE_BADFLAGS:
			debug("Switch to fullscreen: An invalid set of flags was passed in.\n");
			return false;
		case DISP_CHANGE_BADPARAM:
			debug("Switch to fullscreen: An invalid parameter was passed in. This can include an invalid flag or combination of flags.\n");
			return false;
		case DISP_CHANGE_FAILED:
			debug("Switch to fullscreen: The display driver failed the specified graphics mode.\n");
			return false;
		case DISP_CHANGE_BADMODE:
			debug("Switch to fullscreen: The graphics mode is not supported.\n");
			return false;
	}

	debug("An unknown error occured while changing to fullscreen.\n");
	return false;
}
bool Core::createGLRender(SCreateParams &params)
{
	static	PIXELFORMATDESCRIPTOR pfd =	{
		sizeof(PIXELFORMATDESCRIPTOR),	// Size Of This Pixel Format Descriptor
		1,							// Version Number
		PFD_DRAW_TO_WINDOW |		// Format Must Support Window
		PFD_SUPPORT_OPENGL |		// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,			// Must Support Double Buffering
		PFD_TYPE_RGBA,				// Request An RGBA Format
		params.mBitsPerPixel,		// Select Our Color Depth
		0, 0, 0, 0, 0, 0,			// Color Bits Ignored
		0,							// No Alpha Buffer
		0,							// Shift Bit Ignored
		0,							// No Accumulation Buffer
		0, 0, 0, 0,					// Accumulation Bits Ignored
		params.mDepthBits,			// 16Bit Z-Buffer (Depth Buffer)
		params.mStencilBits,		// Stencil Buffer
		0,							// No Auxiliary Buffer
		PFD_MAIN_PLANE,				// Main Drawing Layer
		0,							// Reserved
		0, 0, 0						// Layer Masks Ignored
	};

	GLuint PixelFormat;

	hDC = GetDC(hWnd);
	// choose pixelformat
	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))
	{
		debug("Cannot find a suitable pixelformat.\n");
		return false;
	}

	// set pixel format
	if(!SetPixelFormat(hDC, PixelFormat, &pfd))
	{
		debug("Cannot set the pixel format.\n");
		return false;
	}

	// create rendering context
	if (!(hRC=wglCreateContext(hDC)))
	{
		debug("Cannot create a GL rendering context.\n");
		return false;
	}

	// activate rendering context
	if(!wglMakeCurrent(hDC, hRC))
	{
		debug("Cannot activate GL rendering context.\n");
		return false;
	}

	// print renderer information
	debug("RENDERER:%s\nVENDOR:%s\n", glGetString(GL_RENDERER), glGetString(GL_VENDOR));


	// load extensions
	//if(!glewIsInitialized)
	//{
	glewInit();
	//glewIsInitialized = true;
	//}
	debug("OpenGL VERSION:%s\n", glGetString(GL_VERSION));

	glViewport(0, 0, mParams.mWidth, mParams.mHeight); // Reset The Current Viewport
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_DEPTH_TEST);

	// set vsync
	if (WGLEW_EXT_swap_control)
		wglSwapIntervalEXT(mParams.mVSync ? 1 : 0);

	// set the renderstates

	return true;
}

LRESULT CALLBACK Core::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	#ifndef WM_MOUSEWHEEL
	#define WM_MOUSEWHEEL 0x020A
	#endif
	#ifndef WHEEL_DELTA                     
	#define WHEEL_DELTA 120
	#endif

	Core* core = 0;
	core = mEnvMap[hWnd];
	float wheelDelta, x, y;
	uint32 width, height;

	switch (message) 
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
		}
		return 0;

	case WM_ERASEBKGND:
		return 0;

	case WM_SETCURSOR: 
		//envm = getEnvMapperFromHWnd(hWnd);
		//if (envm &&	!envm->irrDev->getWin32CursorControl()->isVisible())
		//{
		//	SetCursor(NULL); 
		//	return 0; 
		//}
        break;

	case WM_MOUSEWHEEL: 
		wheelDelta = ((float)HIWORD(wParam)) / (float)WHEEL_DELTA;
		if( core )
			core->mReceiver->OnMouseWheel(wheelDelta);
		break;

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
		x = (float)LOWORD(lParam);
		y = (float)HIWORD(lParam);
		if( core )
			core->mReceiver->OnMouseDown(x, y, wParam);
		return 0;

	case WM_LBUTTONUP:
		wParam |= MK_LBUTTON;
	case WM_RBUTTONUP:
		wParam |= MK_RBUTTON;
	case WM_MBUTTONUP:
		wParam |= MK_MBUTTON;
		x = (float)LOWORD(lParam);
		y = (float)HIWORD(lParam);
		if( core )
			core->mReceiver->OnMouseUp(x, y, wParam);
		return 0;

	case WM_MOUSEMOVE:
		x = (float)LOWORD(lParam);
		y = (float)HIWORD(lParam);
		if( core )
			core->mReceiver->OnMouseMove(x, y);
		return 0;

	case WM_KEYDOWN:
		{
			//event.EventType = irr::EET_KEY_INPUT_EVENT;
			//event.KeyInput.Key = (irr::EKEY_CODE)wParam;
			//event.KeyInput.PressedDown = true;
			//dev = getDeviceFromHWnd(hWnd);

			//BYTE allKeys[256];
			//WORD KeyAsc=0;
			//GetKeyboardState(allKeys);
			//ToAscii(wParam,lParam,allKeys,&KeyAsc,0);

			//event.KeyInput.Shift = ((allKeys[VK_SHIFT] & 0x80)!=0);
			//event.KeyInput.Control = ((allKeys[VK_CONTROL] & 0x80)!=0);
			//event.KeyInput.Char = KeyAsc; //KeyAsc >= 0 ? KeyAsc : 0;

			//if (dev)
			//	dev->postEventFromUser(event);

			return 0;
		}
	case WM_KEYUP:
		{
			//event.EventType = irr::EET_KEY_INPUT_EVENT;
			//event.KeyInput.Key = (irr::EKEY_CODE)wParam;
			//event.KeyInput.PressedDown = false;
			//dev = getDeviceFromHWnd(hWnd);

			//BYTE allKeys[256];
			//WORD KeyAsc=0;
			//GetKeyboardState(allKeys);
			//ToAscii(wParam,lParam,allKeys,&KeyAsc,0);

			//event.KeyInput.Shift = ((allKeys[VK_SHIFT] & 0x80)!=0);
			//event.KeyInput.Control = ((allKeys[VK_CONTROL] & 0x80)!=0);
			//event.KeyInput.Char = KeyAsc; //KeyAsc >= 0 ? KeyAsc : 0;

			//if (dev)
			//	dev->postEventFromUser(event);

			return 0;
		}

	case WM_SIZE:
		width = LOWORD(lParam);
		height = HIWORD(lParam);
		if( core )
			core->mReceiver->OnResize(width, height);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_SYSCOMMAND:
		// prevent screensaver or monitor powersave mode from starting
		if (wParam == SC_SCREENSAVE ||
			wParam == SC_MONITORPOWER)
			return 0;
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}