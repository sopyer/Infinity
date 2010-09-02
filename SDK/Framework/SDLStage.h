#ifndef __SDLSTAGE_H_INCLUDED__
#	define __SDLSTAGE_H_INCLUDED__

#include <utils.h>
#include <Events.h>
#include <Stage.h>

struct SDL_Surface;

namespace UI
{
	//! Refactor initializing code
	class SDLStage: public Stage
	{
		public:
			SDLStage();
			~SDLStage();
			
			void run();
			void close();
			
		protected:
			bool	mFullscreen;

			Timer	mTimer;

		private:
			void handleInput();
			void handleRender();
		
		private:
			SDL_Surface *mScreen;
	};
}

#endif