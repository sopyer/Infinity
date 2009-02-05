#include <Scheduler.h>

//! Refactor initializing code
class Stage: public Singleton<Stage>, public UI::Container
{
	private:
		enum Phase
		{
			PHASE_DEFAULT,
			PHASE_ALLOCATE,
			PHASE_EVENT_HANDLING,
			PHASE_RENDERING,
			PHASE_PICK
		};

	public:
		Stage();
		~Stage();
		
		Stage& queueRelayout() {if (mPhase != PHASE_ALLOCATE) {mDoAllocate = true;}	return *this;}
		
		Stage& captureFocus(Actor* focused)
		{
			if (mFocused != focused)
			{
				if (mFocused) mFocused->onFocusOut();
				mFocused = focused;
				if (mFocused) mFocused->onFocusIn();
			}
			return *this;
		}
		Stage& releaseFocus()
		{
			if (mFocused) mFocused->onFocusOut();
			mFocused = 0; return *this;
		}

		virtual void run();

	protected:
		virtual void paint() {}//make obsolete!!!!!!!!!!!!!!!!!!
	
		void close();
		
		//Legacy interface
		virtual void OnCreate(){}
		virtual void OnDestroy(){}

	private:
		void enterPhase(Phase nextPhase) {mPhase = nextPhase;}
		void loadConfig(const char* cfgFile);

		static int	closeCallback();
		static void	buttonCallback(int button, int action);
		static void	moveCallback(int x, int y);
		static void	keyCallback(int key, int action);

	protected:
		int		mBitsPerPixel;
		int		mDepthBits;
		int		mStencilBits;
		bool	mFullscreen;
		bool	mVSync;
		
		bool	mDoAllocate;

		Timer	mTimer;

	private:
		TextureManager	mTextureMgr;
		FontManager		mFontMgr;
		ProgramManager	mProgramMgr;
		ShaderManager	mShaderMgr;

		Scheduler	mScheduler;

		bool	mRunning;
		Phase	mPhase;

		Actor*	mLastVisited;
		Actor*	mFocused;

		char mPaths[1024];	//! Is's terrible;
};
