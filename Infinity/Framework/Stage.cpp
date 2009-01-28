#define GLEW_STATIC
#include <windows.h>
#include <iniparser.h>
#include <cstdlib>
#include <gl\glew.h>
#include "UI.h"
#include "Framework.h"

namespace UI
{
	void Stage::loadConfig(const char* cfgFile)
	{
		dictionary * dict;
		
		dict = iniparser_load(cfgFile);
		
		mWidth  = (float)iniparser_getint(dict, "Video:Width", 800);
		mHeight = (float)iniparser_getint(dict, "Video:Height", 600);
		
		mBitsPerPixel = iniparser_getint(dict, "Video:BitsPerPixel", 32);
		mDepthBits    = iniparser_getint(dict, "Video:DepthBits", 24);
		mStencilBits  = iniparser_getint(dict, "Video:StencilBits", 8);
		
		mFullscreen = iniparser_getboolean(dict, "Video:Fullscreen", false) != 0;
		mVSync      = iniparser_getboolean(dict, "Video:VSync", false) != 0;
		
		strcpy(mPaths, iniparser_getstring(dict, "General:Paths", ""));
		
		iniparser_freedict(dict);
	}

	Stage::Stage(): mFocused(0), mDoAllocate(true), mLastVisited(0), mPhase(PHASE_DEFAULT)
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

		if (!glfwOpenWindow((int)mWidth, (int)mHeight, 
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
		int	x, y;
		glfwGetMousePos(&x, &y);

		if (Actor*	actor = Stage::getRef().pick((float)x, (float)y))
		{
			if (action == GLFW_PRESS)
			{
				//Stage::getRef().onTouch(pos, (uint32)button);
				actor->onTouch(0, 0, (uint32)button); //FIX THIS!!!!!!!!!!!!!!!!!!!!!!!!!!
			}
			else if (action == GLFW_RELEASE)
			{
				//Stage::getRef().onUntouch(pos, (uint32)button);
				actor->onUntouch(0, 0, (uint32)button); //FIX THIS!!!!!!!!!!!!!!!!!!!!!!!!!!
			}
			else
			{
				assert(0);
			}
		}
	}

	void Stage::moveCallback(int x, int y)
	{
		Actor*	actor = Stage::getRef().pick((float)x, (float)y);

		if (Stage::getRef().mLastVisited != actor)
		{
			if (Stage::getRef().mLastVisited)
				Stage::getRef().mLastVisited->onLeave();
			if (actor)
				actor->onEnter();
		}

		if (actor)
			actor->onMotion(0, 0); //FIX THIS!!!!!!!!!!!!!!!!!!!!!!!!!!

		Stage::getRef().mLastVisited = actor;
	}

	void Stage::keyCallback(int key, int action)
	{
		if (Stage::getRef().mFocused)
		{
			if (action == GLFW_PRESS)
			{
				Stage::getRef().mFocused->onKeyDown((uint32)key);
			}
			else if (action == GLFW_RELEASE)
			{
				Stage::getRef().mFocused->onKeyUp((uint32)key);
			}
			else
			{
				assert(0);
			}
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
			mPhase = PHASE_EVENT_HANDLING;
			mScheduler.mainLoop();

			mTimer.resetFrame();
			
			if (mDoAllocate)
			{
				mPhase = PHASE_ALLOCATE;
				mDoAllocate = false;
				onAllocate();
			}
			
			mPhase = PHASE_RENDERING;
			paint();
			glfwSwapBuffers();
		}

		OnDestroy();
	}
}