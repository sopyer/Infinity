#include <Scheduler.h>
#include <queue>

namespace UI
{
	//! Refactor initializing code
	class Stage: public Singleton<Stage>,
		public Container, public sigslot::has_slots<>
	{
		public:
			Stage();
			~Stage();
			
			Stage& queueRelayout();
			
			Stage& captureFocus(Actor* focused);
			Stage& releaseFocus();

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
			enum Phase
			{
				PHASE_DEFAULT,
				PHASE_ALLOCATE,
				PHASE_EVENT_HANDLING,
				PHASE_RENDERING,
				PHASE_PICK
			};
			
			struct KeyQueuedEvent
			{
				KeyEvent	event;
				int			action;
			};

		private:
			struct RenderItem
			{
				glm::mat4	transform;
				Actor*		actor;
			};

		private:
			void enterPhase(Phase nextPhase) {mPhase = nextPhase;}
			void loadConfig(const char* cfgFile);

			static int	GLFWCALL closeCallback();
			static void GLFWCALL charCallback(int unicode, int action);
			static void	GLFWCALL keyCallback(int key, int action);

			void handleKeybdEvents();
			void handleMouseEvents();
			void handleInput(u32 frame);
		
			Actor* doPick(u32 x, u32 y);

			void renderActors();
			void updateRenderQueue(Container* container, const glm::mat4& parentTransform);

			//virtual void onTouch(const ButtonEvent& event);
			//virtual void onUntouch(const ButtonEvent& event);
			//virtual void onMotion(const MotionEvent& event);

		private:
			VFS				mVFS;
			//TextureManager	mTextureMgr;
			FontManager		mFontMgr;
			ProgramManager	mProgramMgr;
			ShaderManager	mShaderMgr;
	
			VG				mVG;

			Scheduler	mScheduler;
			Timeline	mInputTicker;

			bool	mDoAllocate;
			bool	mRunning;
			Phase	mPhase;

			Actor*	mLastVisited;
			Actor*	mFocused;

			int		mPrevButtonState[MouseButtons::Count];
			int		mModifiers;
			float	mPrevX, mPrevY;
			
			bool		mAutoRepeat;
			u32			mAutoRepeatTime;
			KeyEvent	mEvtKeyAutoRepeat;

			std::queue<KeyQueuedEvent>	mKeyEvents;
			
			std::vector<RenderItem>		mRenderQueue;

			char mPaths[1024];	//! Is's terrible;
	};
}