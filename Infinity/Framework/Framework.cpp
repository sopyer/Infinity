#include <windows.h>
#include <iniparser.h>
#include <cstdlib>
#include "Framework.h"

void Stage::loadConfig(const char* cfgFile)
{
	dictionary * dict;
	
	dict = iniparser_load(cfgFile);
	
	mWidth  = iniparser_getint(dict, "Video:Width", 800);
	mHeight = iniparser_getint(dict, "Video:Height", 600);
	
	mBitsPerPixel = iniparser_getint(dict, "Video:BitsPerPixel", 32);
	mDepthBits    = iniparser_getint(dict, "Video:DepthBits", 24);
	mStencilBits  = iniparser_getint(dict, "Video:StencilBits", 8);
	
	mFullscreen = iniparser_getboolean(dict, "Video:Fullscreen", false) != 0;
	mVSync      = iniparser_getboolean(dict, "Video:VSync", false) != 0;
	
	strcpy(mPaths, iniparser_getstring(dict, "General:Paths", ""));
	
	iniparser_freedict(dict);
}

Stage::Stage(): mWidth(mSize.x), mHeight(mSize.y)
{
	mRunning = false;

	AllocConsole();

	vfsInit();
	glfwInit();

	loadConfig("infinity.cfg");

	logMessage("Resources paths initializing...");
	char* path=strtok(mPaths, ";");
	while( path )
	{
		if( strstr(path, ".zip" ) )
			vfsAddZip(path);
		else
			vfsAddRoot(path);
		path = strtok(0, ";");
	}

	if (!mVSync) glfwOpenWindowHint(GLFW_REFRESH_RATE, 0);

	if (!glfwOpenWindow(mWidth, mHeight, 
						0, 0, 0, 0, 
						mDepthBits, mStencilBits, 
						mFullscreen ? GLFW_FULLSCREEN : GLFW_WINDOW))
	{
		glfwTerminate();
		return;
	}

	glfwSetWindowCloseCallback(closeCallback);
	glfwSetMouseButtonCallback(buttonCallback);
	glfwSetMousePosCallback(moveCallback);
	glfwSetKeyCallback(keyCallback);

	glewInit();

	mTimer.start();
}

Stage::~Stage()
{
	mTimer.stop();
	glfwTerminate();
	vfsTerminate();
}

int Stage::closeCallback()
{
	Stage::getRef().close();
	return GL_FALSE;
}

void Stage::buttonCallback(int button, int action)
{
	glm::ivec2	pos;
	glfwGetMousePos(&pos.x, &pos.y);

	if (action == GLFW_PRESS)
	{
		Stage::getRef().onTouch(pos, (uint32)button);
	}
	else if (action == GLFW_RELEASE)
	{
		Stage::getRef().onUntouch(pos, (uint32)button);
	}
	else
	{
		assert(0);
	}
}

void Stage::moveCallback(int x, int y)
{
	glm::ivec2	pos(x, y);

	Stage::getRef().onMotion(pos);
}

void Stage::keyCallback(int key, int action)
{
	if (action == GLFW_PRESS)
	{
		Stage::getRef().onKeyDown((uint32)key);
	}
	else if (action == GLFW_RELEASE)
	{
		Stage::getRef().onKeyUp((uint32)key);
	}
	else
	{
		assert(0);
	}
}


void Stage::close()
{
	mRunning = false;
}

void Stage::run()
{
	OnCreate();

	mRunning = true;

	mTimer.resetFrame();
	while (mRunning && glfwGetWindowParam(GLFW_OPENED))
	{
		onUpdate(0);
		mTimer.resetFrame();

		paint();
		glfwSwapBuffers();
	}

	OnDestroy();
}

void logMessage(const char *s,...)
{
	char str[256];
	DWORD written=0;
	va_list vl;
	va_start(vl, s);
	vsnprintf(str, 255, s, vl);
	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), str, strlen(str), &written, 0);
	OutputDebugString(str);
	va_end(vl);
}
