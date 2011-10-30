#ifndef __SDLSTAGE_H_INCLUDED__
#	define __SDLSTAGE_H_INCLUDED__

#include <utils.h>
#include <Events.h>
#include <Stage.h>

struct SDL_Surface;
class  ShaderEditOverlay;

namespace ui
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
			//Debug functions and callbacks
			void addPrograms(size_t count, GLuint* programs);
			virtual void onShaderRecompile() {}

		protected:
			bool	mFullscreen;

		private:
			void handleInput();
			void handleRender();
		
		private:
			ShaderEditOverlay*	mShaderEditOverlay;
			SDL_Surface			*mScreen;
			bool				mShaderEditOverlayVisible;
	};
}

#endif