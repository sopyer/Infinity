#pragma once

#include <gl\glew.h>

#include "Log.h"
#include "ConsoleSink.h"
#include "FileSink.h"
#include "vfs.h"

#include "Timer.h"
#include "ResourceManager.h"
#include "Core.h"
#include "Config.h"
#include "Camera.h"

class Engine: public IEventReceiver
{
private:
	void loadConfig(std::string cfgName, Core::SCreateParams &params)
	{
		Config cfg;
		cfg.load(cfgName);
		params.mWidth = cfg.get_int("Video", "Width", 1024);
		params.mHeight = cfg.get_int("Video", "Height", 768);
		params.mBitsPerPixel = cfg.get_int("Video", "BitsPerPixel", 32);
		params.mDepthBits = cfg.get_int("Video", "DepthBits", 24);
		params.mStencilBits = cfg.get_int("Video", "StencilBits", 8);
		params.mFullscreen = cfg.get_boolean("Video", "Fullscreen", false);
		params.mVSync = cfg.get_boolean("Video", "VSync", false);
		debug("Resolution:%dx%d\n", params.mWidth, params.mHeight);
		debug("Color, stencil, depth: %d, %d, %d\n", params.mBitsPerPixel, params.mStencilBits, params.mDepthBits);
		debug("Fullscreen - %s\n", params.mFullscreen?"yes":"no");
		debug("VSync - %s\n", params.mVSync?"yes":"no");
	}

public:
	Engine():m_RM(m_VFS)
	{
		m_Log.AttachSink(LogSinkPtr(new ConsoleSink));
		m_Log.AttachSink(LogSinkPtr(new FileSink("darkness_log.txt")));
		m_Log.SetOutputLevel(Log::Debug);
		loadConfig("data\\ogl.cfg", coreParams);
		m_VFS.AddRootPath(".\\data");
		if( !mCore.create(coreParams) )
			m_Log.Print(Log::Error, "Core object has not been created.\n");
		m_Log.Print(Log::Info, "Core object has been created.\n");
		
		mCore.setEventReceiver(this);
		float aspect=(float)coreParams.mWidth/coreParams.mHeight;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0, aspect,1,2048); 
		glMatrixMode(GL_MODELVIEW);
	}

	~Engine()
	{
		mCore.destroy();
	}

	WPARAM MainLoop()
	{
		char fps[50];
		OnCreate();
		timer.Start();
		float t=timer.GetTime();
		while( mCore.run() )
		{
			OnUpdate(timer.GetTime());
			OnRender();
			if(timer.GetDeltaTime()>1)
			{
				debug("smth. wrong\n");
			}
			timer.Frame();
			//check only for caption output
			if(timer.GetTime()-t>0.7)
			{

				t=timer.GetTime();
				sprintf(fps, "FPS:%0.0f", timer.GetFps());
				//SetWindowText(m_Wnd.GetHandle(), fps);
				mCore.setWindowCaption(fps);
			}
			mCore.present();
		}
		OnDestroy();
		return 0;
	}

	virtual void OnCreate(){}
	virtual void OnDestroy(){}
	virtual void OnUpdate(float Time){}
	virtual void OnRender(){}
	virtual void OnResize(uint32 newWidth, uint32 newHeight){}
	virtual void OnMouseUp(float x, float y, uint32 state){}
	virtual void OnMouseDown(float x, float y, uint32 state){}
	virtual void OnMouseMove(float x, float y){}
	virtual void OnMouseWheel(float delta){}
	virtual void OnKeyUp(uint32 key, uint32 funcState){}
	virtual void OnKeyDown(uint32 key, uint32 funcState){}
public:
	Core::SCreateParams coreParams; 
	Log	m_Log;
	VFS m_VFS;
	Core	mCore;
	Timer	timer;
	ResourceManager	m_RM;
};