#include <windows.h>
#include <iniparser.h>
#include <cstdlib>
#include "Framework.h"

static char str[256];

void print(const char *s,...)
{
	DWORD written=0;
	va_list vl;
	va_start(vl, s);
	vsnprintf(str, 255, s, vl);
	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), str, strlen(str), &written, 0);
	OutputDebugString(str);
	va_end(vl);
}


void Framework::loadConfig(const char* cfgFile)
{
	dictionary * dico;		// The dictionnary containing the parsed data

	dico = iniparser_load(cfgFile);
	width_ = iniparser_getint(dico, "Video:Width", 800);
	height_ = iniparser_getint(dico, "Video:Height", 600);
	//params.mBitsPerPixel = cfg.get_int("Video", "BitsPerPixel", 32);
	depthBits_ = iniparser_getint(dico, "Video:DepthBits", 24);
	stencilBits_ = iniparser_getint(dico, "Video:StencilBits", 8);
	fullscreen_ = iniparser_getboolean(dico, "Video:Fullscreen", false)>0;//to fix stupid MSVC warnings
	vsync_ = iniparser_getboolean(dico, "Video:VSync", false)>0;//so and here

	iniparser_freedict(dico);
}

Framework::Framework()
{
	running_ = false;
	AllocConsole();
	vfsInit();
	glfwInit();
	loadConfig("video.cfg");
	if( !vsync_ )
		glfwOpenWindowHint(GLFW_REFRESH_RATE, 0);
	if( !glfwOpenWindow( width_ , height_, 0,0,0,0,depthBits_,stencilBits_, fullscreen_?GLFW_FULLSCREEN:GLFW_WINDOW ) )
	{
		glfwTerminate();
		return;
	}
	glfwSetWindowCloseCallback(closeCallback);
	glewInit();
}

Framework::~Framework()
{
	glfwTerminate();
	vfsTerminate();
}

int GLFWCALL Framework::closeCallback()
{
	Framework::getRef().OnDestroy();
	return GL_TRUE;
}

void Framework::close()
{
	running_ = false;
}

void Framework::run()
{
	// Main loop
	OnCreate();
	
	running_ = true;
	while( running_ && glfwGetWindowParam( GLFW_OPENED ) )
	{
		OnUpdate((float)glfwGetTime());

		OnRender();
		// Swap front and back rendering buffers
		glfwSwapBuffers();
		// Check if ESC key was pressed or window was closed
	}

	if( glfwGetWindowParam( GLFW_OPENED ) )
		OnDestroy();
}