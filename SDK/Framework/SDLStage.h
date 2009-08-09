#ifndef __SDLSTAGE_H_INCLUDED__
#	define __SDLSTAGE_H_INCLUDED__

#include <utils.h>
#include <Events.h>
#include <Stage.h>

struct SDL_Surface;

namespace UI
{
	//! Refactor initializing code
	class SDLStage: public Stage, public Singleton<GLFWStage>
	{
		public:
			SDLStage();
			~SDLStage();
			
			void run();
			void close();
			
			bool isRunning() {return mRunning;}

		protected:
			int		mBitsPerPixel;
			int		mDepthBits;
			int		mStencilBits;
			bool	mFullscreen;
			bool	mVSync;

			Timer	mTimer;

		private:
			void loadConfig(const char* cfgFile);
			void handleInput(u32 frame);
		
		private:
			VFS				mVFS;
			FontManager		mFontMgr;
			ProgramManager	mProgramMgr;
			ShaderManager	mShaderMgr;
	
			Scheduler	mScheduler;
			Timeline	mInputTicker;

			bool	mRunning;
			
			SDL_Surface *mScreen;
			
			char mPaths[1024];	//! Is's terrible;
	};
}

#endif