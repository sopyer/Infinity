#include <gl\glee.h>
#include "UI.h"
#include "Framework.h"
#include "SDLStage.h"
#include <windows.h>
#include <iniparser.h>
#include <cstdlib>
#include <SOIL.h> // remove later
#include <SDL.h>

namespace UI
{
	void SDLStage::loadConfig(const char* cfgFile)
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

	SDLStage::SDLStage(): mInputTicker(50)
	{
		mRunning = false;
		AllocConsole();

		if(SDL_Init(SDL_INIT_VIDEO)<0)											// Init The SDL Library, The VIDEO Subsystem
		{
			logMessage("Unable to open SDL: %s\n", SDL_GetError() );					// If SDL Can't Be Initialized
			exit(1);															// Get Out Of Here. Sorry.
		}

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

		u32 flags = SDL_HWSURFACE|SDL_OPENGLBLIT;									// We Want A Hardware Surface And Special OpenGLBlit Mode
		if (mFullscreen)		
		{
			flags |= SDL_FULLSCREEN;
		}

		SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );								// In order to use SDL_OPENGLBLIT we have to
		SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );							// set GL attributes first
		SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
		SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
		SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
		SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8 );
		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );							// colors and doublebuffering
		SDL_GL_SetAttribute( SDL_GL_SWAP_CONTROL, mVSync );							// colors and doublebuffering

		if(!(mScreen = SDL_SetVideoMode((int)mWidth, (int)mHeight, 32, flags)))
		{
			logMessage("Unable to open screen surface: %s\n", SDL_GetError() );		// If Something's Gone Wrong, Report
			exit(1);															// And Exit
		}

		//if (!mVSync) glfwOpenWindowHint(GLFW_REFRESH_RATE, 0);

		//if (!glfwOpenWindow((int)mWidth, (int)mHeight, 
		//					8,8,8,8,//0, 0, 0, 0, 
		//					mDepthBits, mStencilBits, 
		//					mFullscreen ? GLFW_FULLSCREEN : GLFW_WINDOW))
		//{
		//	glfwTerminate();
		//	return;
		//}
		
		//glfwSetWindowCloseCallback(SDLStage::closeCallback);
		//glfwSetKeyCallback(SDLStage::keyCallback);
		//glfwSetCharCallback(SDLStage::charCallback);

		mVG.create();
		mVG.setSize((GLuint)mWidth, (GLuint)mHeight);

		mTimer.start();
		mInputTicker.onFrame.connect(this, &SDLStage::handleInput);
		mInputTicker.start();

		//setPos(-mWidth/2, -mHeight/2); //Fix if center of coords is in center of screen
	}

	SDLStage::~SDLStage()
	{
		mTimer.stop();
		SDL_Quit();
	}

	void SDLStage::handleInput(u32	frame)
	{
		enterPhase(PHASE_EVENT_HANDLING);
		SDL_Event	E;
		u32 i = 0;
		while (SDL_PollEvent(&E) && i<10)
		{
			switch(E.type)
			{
				case SDL_QUIT:
					close();
					break;

				case SDL_KEYDOWN:		
					break;
				case SDL_KEYUP:		
					break;
				case SDL_MOUSEMOTION:
					break;
				case SDL_MOUSEBUTTONDOWN:
					break;
				case SDL_MOUSEBUTTONUP:
					break;
			}
			++i;
		}
	}

	void SDLStage::close()
	{
		mRunning = false;
	}

	void SDLStage::run()
	{
		mRunning = true;

		mTimer.resetFrame();
		while (mRunning /*&& glfwGetWindowParam(GLFW_OPENED)*/)
		{
			mScheduler.mainLoop();
			mTimer.resetFrame();
			enterPhase(PHASE_ALLOCATE);
			enterPhase(PHASE_RENDERING);
			enterPhase(PHASE_DEFAULT);
			SDL_GL_SwapBuffers();										// And Swap The Buffers (We're Double-Buffering, Remember?)
		}
	}

}