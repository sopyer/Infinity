#include <gl\glee.h>
#include "UI.h"
#include "Framework.h"
#include "GLFWStage.h"
#include <windows.h>
#include <iniparser.h>
#include <cstdlib>
#include <SOIL.h> // remove later

namespace UI
{
	void GLFWStage::loadConfig(const char* cfgFile)
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

	GLFWStage::GLFWStage(): mInputTicker(50)
	{
		mRunning = false;
		AllocConsole();
		glfwInit();
		loadConfig("infinity.cfg");
		logMessage("Resources paths initializing...");

		char* path=strtok(mPaths, ";");
		while( path )
		{
			if( strstr(path, ".zip" ) )
				VFS::mount(path);
			else
				VFS::mount(path);

			path = strtok(0, ";");
		}

		if (!mVSync) glfwOpenWindowHint(GLFW_REFRESH_RATE, 0);

		if (!glfwOpenWindow((int)mWidth, (int)mHeight, 
							8,8,8,8,//0, 0, 0, 0, 
							mDepthBits, mStencilBits, 
							mFullscreen ? GLFW_FULLSCREEN : GLFW_WINDOW))
		{
			glfwTerminate();
			return;
		}
		
		glfwSetWindowCloseCallback(GLFWStage::closeCallback);
		glfwSetKeyCallback(GLFWStage::keyCallback);
		glfwSetCharCallback(GLFWStage::charCallback);

		mVG.create();
		mVG.setSize((GLuint)mWidth, (GLuint)mHeight);

		mTimer.start();
		mInputTicker.onFrame.connect(this, &GLFWStage::handleInput);
		mInputTicker.start();

		//setPos(-mWidth/2, -mHeight/2); //Fix if center of coords is in center of screen
	}

	GLFWStage::~GLFWStage()
	{
		mTimer.stop();
		glfwTerminate();
	}

	int GLFWCALL GLFWStage::closeCallback()
	{
		GLFWStage::getRef().close();
		return GL_FALSE;
	}

	void GLFWCALL GLFWStage::charCallback(int unicode, int action)
	{
		assert(action == InputState::Pressed || action == InputState::Released);
		assert(!GLFWStage::getRef().mKeyEvents.empty());
		assert(GLFWStage::getRef().mKeyEvents.back().action == action);

		GLFWStage::getRef().mKeyEvents.back().event.unicode = unicode;
	}

	void GLFWCALL GLFWStage::keyCallback(int key, int action)
	{
		assert(action == InputState::Pressed || action == InputState::Released);

		KeyEvent		event  = {key, GLFWStage::getRef().mModifiers, 0};
		KeyQueuedEvent	qevent = {event, action};
		
		GLFWStage::getRef().mKeyEvents.push(qevent);
	}

	void GLFWStage::handleKeybdEvents()
	{
		static bool firstTime = true;
		if (firstTime)
		{
			mAutoRepeat = false;
			firstTime = false;
		}

		while (!mKeyEvents.empty())
		{
			KeyEvent&	event = mKeyEvents.front().event;
			if (mKeyEvents.front().action == InputState::Pressed)
			{
				mAutoRepeat = true;
				mAutoRepeatTime = mTimer.getTime() + 500;
				mEvtKeyAutoRepeat = event;
			}
			else if (mKeyEvents.front().action == InputState::Released)
			{
				if (mAutoRepeat && mEvtKeyAutoRepeat.key == mKeyEvents.front().event.key)
					mAutoRepeat = false;
			}

			if (mKeyEvents.front().action == InputState::Pressed)
				onKeyDown(event);
			else if (mKeyEvents.front().action == InputState::Released)
				onKeyUp(event);

			mKeyEvents.pop();
		}

		if (mAutoRepeat && mTimer.getTime() >= mAutoRepeatTime)
		{
			onKeyDown(mEvtKeyAutoRepeat);
			mAutoRepeatTime += 70;
		}
	}

	void GLFWStage::handleMouseEvents()
	{
		int tx, ty;

		glfwGetMousePos(&tx, &ty);
		
		//Quick fix for strange behaviour of GLFW
		tx = glm::clamp<int>(tx, 0, (int)mWidth-1);
		ty = glm::clamp<int>(ty, 0, (int)mHeight-1);

		int modState;
		float x = (float)tx, y = (float)ty;

		static bool firstTime = true;
		if (firstTime)
		{
			memset(mPrevButtonState, 0, sizeof(mPrevButtonState));
			mPrevX = x;
			mPrevY = y;

			firstTime = false;
		}

		//perform picking rendering

		mModifiers = 0;

		modState = glfwGetKey(KeySyms::KeyLCtrl)  | glfwGetKey(KeySyms::KeyRCtrl);
		if (modState == InputState::Pressed)
			mModifiers |= Modifiers::Ctrl;

		modState = glfwGetKey(KeySyms::KeyLAlt)   | glfwGetKey(KeySyms::KeyRAlt);
		if (modState == InputState::Pressed)
			mModifiers |= Modifiers::Alt;

		modState = glfwGetKey(KeySyms::KeyLShift) | glfwGetKey(KeySyms::KeyRShift);
		if (modState == InputState::Pressed)
			mModifiers |= Modifiers::Shift;

		for (int button = 0; button < MouseButtons::Count; ++button)
		{
			int& prevState = mPrevButtonState[button];
			int  curState  = glfwGetMouseButton(button);
			
			if (prevState != curState)
			{
				ButtonEvent event = {button, mModifiers, x, y};
				
				if (curState == InputState::Pressed)
					onTouch(event);
				else
					onUntouch(event);
			}

			prevState = curState;
		}

		if (x!=mPrevX || y!=mPrevY)
		{
			MotionEvent evtMotion = {mModifiers, x, y};
			onMotion(evtMotion);
		}
	}

	void GLFWStage::handleInput(u32	frame)
	{
		enterPhase(PHASE_EVENT_HANDLING);
		handleKeybdEvents();
		handleMouseEvents();
	}

	void GLFWStage::close()
	{
		mRunning = false;
	}

	void GLFWStage::run()
	{
		mRunning = true;

		mTimer.resetFrame();
		while (mRunning && glfwGetWindowParam(GLFW_OPENED))
		{
			mScheduler.mainLoop();
			mTimer.resetFrame();
			enterPhase(PHASE_ALLOCATE);
			enterPhase(PHASE_RENDERING);
			enterPhase(PHASE_DEFAULT);
			glfwSwapBuffers();
		}
	}

}