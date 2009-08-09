#ifndef __GLFWSTAGE_H_INCLUDED__
#	define __GLFWSTAGE_H_INCLUDED__

#include <utils.h>
#include <Events.h>
#include <Stage.h>

namespace UI
{
	//! Refactor initializing code
	class GLFWStage: public Stage, public Singleton<GLFWStage>
	{
		public:
			GLFWStage();
			~GLFWStage();
			
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
			struct KeyQueuedEvent
			{
				KeyEvent	event;
				int			action;
			};

		private:
			void loadConfig(const char* cfgFile);

			static int	GLFWCALL closeCallback();
			static void GLFWCALL charCallback(int unicode, int action);
			static void	GLFWCALL keyCallback(int key, int action);

			void handleKeybdEvents();
			void handleMouseEvents();
			void handleInput(u32 frame);
		
		private:
			VFS				mVFS;
			FontManager		mFontMgr;
			ProgramManager	mProgramMgr;
			ShaderManager	mShaderMgr;
	
			Scheduler	mScheduler;
			Timeline	mInputTicker;

			bool	mRunning;

			int		mPrevButtonState[MouseButtons::Count];
			int		mModifiers;
			float	mPrevX, mPrevY;
			
			bool		mAutoRepeat;
			u32			mAutoRepeatTime;
			KeyEvent	mEvtKeyAutoRepeat;

			std::queue<KeyQueuedEvent>	mKeyEvents;

			char mPaths[1024];	//! Is's terrible;
	};
}

#endif