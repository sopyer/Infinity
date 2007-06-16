#include <windows.h>
#include <iniparser.h>
#include <cstdlib>
#include "Framework.h"

#ifdef _DEBUG
#	include <string>
#	define START_PROFILE(section) sec=section; time_ = glfwGetTime()
#	define END_PROFILE() print("Section %s in %f sec\n", sec.c_str(), glfwGetTime()-time_)
static double time_=0;
static std::string sec;
#else
#	define START_PROFILE(section)
#	define END_PROFILE()
#endif

static char str[256];
static char paths[1024];

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
	strcpy(paths,iniparser_getstring(dico, "General:Paths", ""));
	iniparser_freedict(dico);
}

Framework::Framework()
{
	running_ = false;
	START_PROFILE("AllocConsole");
	AllocConsole();
	END_PROFILE();
	START_PROFILE("VFS");
	vfsInit();
	END_PROFILE();
	START_PROFILE("glfwInit");
	glfwInit();
	END_PROFILE();
	START_PROFILE("loadConfig");
	loadConfig("infinity.cfg");
	END_PROFILE();
	print("Resources pathes initializing...");
	char* path=strtok(paths, ";");
	while( path )
	{
		if( strstr(path, ".zip" ) )
			vfsAddZip(path);
		else
			vfsAddRoot(path);
		path = strtok(0, ";");
	}

	START_PROFILE("Window");
	if( !vsync_ )
		glfwOpenWindowHint(GLFW_REFRESH_RATE, 0);
	if( !glfwOpenWindow( width_ , height_, 0,0,0,0,depthBits_,stencilBits_, fullscreen_?GLFW_FULLSCREEN:GLFW_WINDOW ) )
	{
		glfwTerminate();
		return;
	}
	glfwSetWindowCloseCallback(closeCallback);
	END_PROFILE();
	START_PROFILE("glewInit");
	glewInit();
	END_PROFILE();
}

Framework::~Framework()
{
	glfwTerminate();
	vfsTerminate();
}

int GLFWCALL Framework::closeCallback()
{
	//Framework::getRef().OnDestroy();
	Framework::getRef().close();
	return GL_FALSE;
}

void Framework::close()
{
	running_ = false;
}

void Framework::run()
{
	print("OnCreate\n");
	// Main loop
	OnCreate();
	print("MainLoop\n");
	running_ = true;
	while( running_ && glfwGetWindowParam( GLFW_OPENED ) )
	{
		print("OnUpdate\n");
		OnUpdate((float)glfwGetTime());

		print("OnRender\n");
		OnRender();
		// Swap front and back rendering buffers
		glfwSwapBuffers();
		// Check if ESC key was pressed or window was closed
	}

	//if( glfwGetWindowParam( GLFW_OPENED ) )
		OnDestroy();
}