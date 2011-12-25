#include <opengl.h>
#include "UI.h"
#include "Framework.h"
#include "SDLStage.h"
#include <windows.h>
#include <cstdlib>
#include <SOIL.h> // remove later
#include <SDL.h>
#include <SDL_syswm.h>
#include <vg/vg.h>
#include <vg/impl/SharedResources.h>
#include <ShaderEditOverlay.h>
#include <ProfilerOverlay.h>
#include <profiler.h>

void Platform_Initialise(HWND hWnd);
void Platform_Finalise();

enum
{
	STATE_DEFAULT,
	STATE_SHADER_EDIT,
	STATE_PROFILER
};

namespace ui
{
	SDLStage::SDLStage(): mState(STATE_DEFAULT)
	{
		if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)<0)											// Init The SDL Library, The VIDEO Subsystem
		{
			logging::message("Unable to open SDL: %s\n", SDL_GetError() );					// If SDL Can't Be Initialized
			exit(1);															// Get Out Of Here. Sorry.
		}

		//!!!! Refactor
		mFullscreen = false;
		mWidth = 1280;
		mHeight = 720;

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
		SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1 );
		SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 4 );
		SDL_GL_SetAttribute( SDL_GL_SWAP_CONTROL, 0 );

		if(!(mScreen = SDL_SetVideoMode((int)mWidth, (int)mHeight, 32, flags)))
		{
			logging::message("Unable to open screen surface: %s\n", SDL_GetError() );		// If Something's Gone Wrong, Report
			exit(1);															// And Exit
		}
		
		SDL_EnableUNICODE(TRUE);
		SDL_EnableKeyRepeat(40, 40);

		setProjection(glm::perspectiveGTX(60.0f, mWidth/mHeight, 0.1f, 600.0f));

		importOpenGL();

		mt::init();
		vg::init();
		ui::init();

		SDL_SysWMinfo info = {{0, 0}, 0, 0};
		SDL_GetWMInfo(&info);
		Platform_Initialise(info.window);

		mShaderEditOverlay = new ShaderEditOverlay;
		mShaderEditOverlay->initialise(mWidth, mHeight);

		mProfilerOverlay = new ProfilerOverlay;
		mProfilerOverlay->initialise(mWidth, mHeight);

		GLuint programs[] = {
			impl::simpleUIProgram, impl::stencilArcAreaProgram, impl::stencilQuadAreaProgram,
			impl::stencilCubicAreaAAProgram, impl::stencilCubicAreaProgram, impl::linGradProgram
		};

		mShaderEditOverlay->addPrograms(ARRAY_SIZE(programs), programs);
	}

	void SDLStage::addPrograms(size_t count, GLuint* programs)
	{
		mShaderEditOverlay->addPrograms(count, programs);
	}

	SDLStage::~SDLStage()
	{
		delete mShaderEditOverlay;
		delete mProfilerOverlay;
		Platform_Finalise();
		ui::cleanup();
		vg::cleanup();
		mt::cleanup();
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
					switch (mState)
					{
						case STATE_DEFAULT:
							processKeyDown(E.key);
							break;
						case STATE_SHADER_EDIT:
							mShaderEditOverlay->handleKeyDown(E.key);
							break;
						case STATE_PROFILER:
							mProfilerOverlay->handleKeyDown(E.key);
							break;
					}
					break;
				case SDL_KEYUP:		
					if (E.key.keysym.sym==SDLK_F5)
					{
						mState = mState==STATE_SHADER_EDIT?STATE_DEFAULT:STATE_SHADER_EDIT;
						if (mState==STATE_SHADER_EDIT) mShaderEditOverlay->reset();
					}
					if (E.key.keysym.sym==SDLK_F4)
					{
						mState = mState==STATE_PROFILER?STATE_DEFAULT:STATE_PROFILER;
					}
					if (mState==STATE_DEFAULT)
						processKeyUp(E.key);
					break;
				case SDL_MOUSEMOTION:
					if (mState==STATE_DEFAULT)
						processMotion(E.motion);
					break;
				case SDL_MOUSEBUTTONDOWN:
					if (mState==STATE_DEFAULT)
						processTouch(E.button);
					break;
				case SDL_MOUSEBUTTONUP:
					if (mState==STATE_DEFAULT)
						processUntouch(E.button);
					break;
			}
			++i;
		}
	}

	void SDLStage::handleRender()
	{
		if (mShaderEditOverlay->requireReset())
		{
			impl::readyProgramsForUse();
			onShaderRecompile();
		}

		size_t id;

		profilerBeginFrame();
		
		id = profilerStartBlock("STAGE PHASE ALLOCATE");
		enterPhase(PHASE_ALLOCATE);
		profilerEndBlock(id);
		id = profilerStartBlock("STAGE PHASE RENDERING");
		enterPhase(PHASE_RENDERING);
		profilerEndBlock(id);
		id = profilerStartBlock("STAGE PHASE DEFAULT");
		enterPhase(PHASE_DEFAULT);
		profilerEndBlock(id);

		profilerEndFrame();

		if (mState==STATE_PROFILER)
		{
			mProfilerOverlay->update();
			mProfilerOverlay->renderFullscreen();
		}
		if (mState==STATE_SHADER_EDIT)
			mShaderEditOverlay->renderFullscreen();
		SDL_GL_SwapBuffers();
	}

	void SDLStage::close()
	{
		mt::terminateLoop();
	}

	void SDLStage::run()
	{
		mt::addTimedTask<SDLStage, &SDLStage::handleInput>(this, 33);
		mt::addFrameTask<SDLStage, &SDLStage::handleRender>(this);

		mt::mainLoop();
	}

}