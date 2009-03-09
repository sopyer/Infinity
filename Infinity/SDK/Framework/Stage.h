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

		protected:
			int		mBitsPerPixel;
			int		mDepthBits;
			int		mStencilBits;
			bool	mFullscreen;
			bool	mVSync;
			
			bool	mDoAllocate;

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

			static int	closeCallback();
			static void charCallback(int unicode, int action);
			static void	keyCallback(int key, int action);

			void handleKeybdEvents();
			void handleMouseEvents();
			void handleInput(uint32	frame);
		
			Actor* doPick(uint32 x, uint32 y);
			void renderActors();
			void updateRenderQueue(Container* container, const glm::mat4& parentTransform);

			//virtual void onTouch(const ButtonEvent& event);
			//virtual void onUntouch(const ButtonEvent& event);
			//virtual void onMotion(const MotionEvent& event);

		private:
			TextureManager	mTextureMgr;
			FontManager		mFontMgr;
			ProgramManager	mProgramMgr;
			ShaderManager	mShaderMgr;
	
			VG				mVG;

			Scheduler	mScheduler;
			Timeline	mInputTicker;

			bool	mRunning;
			Phase	mPhase;

			Actor*	mLastVisited;
			Actor*	mFocused;

			int		mPrevButtonState[MouseButtons::Count];
			int		mModifiers;
			float	mPrevX, mPrevY;
			
			bool		mAutoRepeat;
			uint32		mAutoRepeatTime;
			KeyEvent	mEvtKeyAutoRepeat;

			std::queue<KeyQueuedEvent>	mKeyEvents;
			
			std::vector<RenderItem>		mRenderQueue;

			char mPaths[1024];	//! Is's terrible;
	};
}