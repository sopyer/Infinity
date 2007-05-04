#ifndef _FRAMEWORK_H_INCLUDED_
#	define _FRAMEWORK_H_INCLUDED_

#include <..\vfs\vfs.h>
#include <glClasses.h>
#include <gl\glfw.h>
#include <loading.h>
#include <Singleton.h>
#include <FirstPersonCamera.h>

class Framework:public Singleton<Framework>
{
	public:
		Framework();
		~Framework();

		virtual void run();

	protected:
		virtual void OnCreate(){}
		virtual void OnDestroy(){}
		virtual void OnRender(){}
		virtual void OnUpdate(float frame_time){}
		void close();
	private:
		void loadConfig(const char* cfgFile);
		static int GLFWCALL closeCallback();
	protected:
		int width_;
		int height_;
		//int bitsPerPixel_;
		int depthBits_;
		int stencilBits_;
		bool fullscreen_;
		bool vsync_;
	private:
		bool running_;
};

void print(const char *s,...);

#endif