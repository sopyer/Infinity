#ifndef _FRAMEWORK_H_INCLUDED_
#	define _FRAMEWORK_H_INCLUDED_

#include <..\vfs\vfs.h>
#include <glClasses.h>
#include <gl\glfw.h>
#include <Singleton.h>
#include <FirstPersonCamera.h>
#include <TextureManager.h>
#include <ProgramManager.h>
#include <ShaderManager.h>
#include <FontManager.h>
#include <VG.h>
#include <utils.h>
#include <Timer.h>
#include <UI.h>

#include <loading.h> //Remove later

//! Refactor initializing code
class Stage: public Singleton<Stage>, public UI::Container/*UI::Container*/
{
	public:
		Stage();
		~Stage();

		virtual void run();

	protected:
		virtual void paint() {}
	
	protected:
		void close();
		
		//Legacy interface
		virtual void OnCreate(){}
		virtual void OnDestroy(){}

	private:
		void loadConfig(const char* cfgFile);

	private:
		static int closeCallback();

		static void buttonCallback(int button, int action);
		static void moveCallback(int x, int y);

		static void keyCallback(int key, int action);

	protected:
		int& mWidth;
		int& mHeight;
		int mBitsPerPixel;
		int mDepthBits;
		int mStencilBits;
		bool mFullscreen;
		bool mVSync;

		Timer	mTimer;

	private:
		TextureManager	mTextureMgr;
		FontManager		mFontMgr;
		ProgramManager	mProgramMgr;
		ShaderManager	mShaderMgr;

		bool mRunning;

		char mPaths[1024];	//! Is's terrible;
};

void logMessage(const char *s,...);

#endif