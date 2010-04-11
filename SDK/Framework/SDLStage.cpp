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
	SDLStage::SDLStage()/*: mInputTicker(50)*/
	{
		//mRunning = false;

		if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)<0)											// Init The SDL Library, The VIDEO Subsystem
		{
			logging::message("Unable to open SDL: %s\n", SDL_GetError() );					// If SDL Can't Be Initialized
			exit(1);															// Get Out Of Here. Sorry.
		}

		//!!!! Refactor
		mFullscreen = false;
		mWidth = 800;
		mHeight = 600;

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
		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, TRUE );							// colors and doublebuffering
		//SDL_GL_SetAttribute( SDL_GL_SWAP_CONTROL, 1 );							// colors and doublebuffering

		if(!(mScreen = SDL_SetVideoMode((int)mWidth, (int)mHeight, 32, flags)))
		{
			logging::message("Unable to open screen surface: %s\n", SDL_GetError() );		// If Something's Gone Wrong, Report
			exit(1);															// And Exit
		}
		
		SDL_EnableUNICODE(TRUE);
		SDL_EnableKeyRepeat(40, 40);

		setProjection(glm::perspectiveGTX(60.0f, mWidth/mHeight, 0.1f, 600.0f));
	}

	SDLStage::~SDLStage()
	{
		SDL_Quit();
	}

	void SDLStage::handleInput()
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
					processKeyDown(E.key);
					break;
				case SDL_KEYUP:		
					processKeyUp(E.key);
					break;
				case SDL_MOUSEMOTION:
					processMotion(E.motion);
					break;
				case SDL_MOUSEBUTTONDOWN:
					processTouch(E.button);
					break;
				case SDL_MOUSEBUTTONUP:
					processUntouch(E.button);
					break;
			}
			++i;
		}
	}

	void SDLStage::handleRender()
	{
		enterPhase(PHASE_ALLOCATE);
		enterPhase(PHASE_RENDERING);
		enterPhase(PHASE_DEFAULT);
		SDL_GL_SwapBuffers();										// And Swap The Buffers (We're Double-Buffering, Remember?)
	}

	void SDLStage::close()
	{
		//mRunning = false;
		scheduler::terminateLoop();
	}

	void SDLStage::run()
	{
		//mRunning = true;

		//mTimer.resetFrame();
		//while (mRunning)
		//{
		//	mScheduler.mainLoop();
		//	mTimer.resetFrame();
		//}
		scheduler::addTimedTask<SDLStage, &SDLStage::handleInput>(this, 33);
		//Do not make this too small ro there will be input handling lag
		scheduler::addTimedTask<SDLStage, &SDLStage::handleRender>(this, 20);

		scheduler::mainLoop();
	}

}