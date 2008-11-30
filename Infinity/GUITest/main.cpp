#include <framework.h>
#include <FTGLTextureFont.h>
#include <GUI.h>
#include <sigslot.h>
#include "klass.h"
class FontApp: public Window/*Framework*/, public sigslot::has_slots<>
{
	private:
		FTFont*	times;
		Button* but;
		Label*	lbl;
		Frame*	frame;
	protected:
		void makeOrtho()
		{
			int			w = width_;
			int			h = height_;
			GLdouble	aspect = (GLdouble)w / (GLdouble)h;
			
			// We are going to do some 2-D orthographic drawing.
			// Use the whole window.
			glViewport(0, 0, w, h);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			GLdouble size = (GLdouble)((w >= h) ? w : h) / 2.0;

			if (w <= h)
			{
				aspect = (GLdouble)h/(GLdouble)w;
				glOrtho(-size, size, -size*aspect, size*aspect, -100000.0, 100000.0);
			}
			else
			{
				aspect = (GLdouble)w/(GLdouble)h;
				glOrtho(-size*aspect, size*aspect, -size, size, -100000.0, 100000.0);
			}

			// Make the world and window coordinates coincide so that 1.0 in
			// model space equals one pixel in window space.
			glScaled(aspect, aspect, 1.0);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
		}

		void OnCreate()
		{
			times = new FTGLTextureFont("C:\\WINDOWS\\Fonts\\times.ttf");
			times->FaceSize(24);
			
			frame = new Frame(gm::rect(glm::ivec2(200, 200), glm::ivec2(266+200, 200+200)), "Hello World", times);
			but = new Button(gm::rect(glm::ivec2(0+120, 0+20), glm::ivec2(110+120, 35+20)), "Quit App!", times);
			but->clicked.connect(this, &FontApp::onClose);
			lbl = new Label(gm::rect(glm::ivec2(0, 570), glm::ivec2(40, 600)), "FPS test string la-la-la-la next line", times);
			frame->add(but);
			add(lbl);
			add(frame);
			makeOrtho();

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);

			glClearDepth(1.0);
			glClearColor(0,0,0,0);
		}
		
		void OnDestroy()
		{
			delete frame;
			delete but;
			delete lbl;
			delete times;
		}

		void OnUpdate(float frame_time)
		{
			if( glfwGetKey(GLFW_KEY_ESC) )
				close();
		}

		void onClose()
		{
			close();
		}
};

int main()
{
	FontApp app;
	app.run();
	return 0;
}