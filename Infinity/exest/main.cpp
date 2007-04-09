#include <framework.h>

class Exest: public Framework
{
	private:
		glRenderer	renderer_;
		glTexture2D	*image_[2];
	protected:
		void OnCreate()
		{
			vfsAddRoot("D:\\Temp\\ExestData");
			image_[0] = loadPngTexture("pngtest.png");
			image_[1] = loadJpegTexture("highlight.jpg");
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(90.0, (float)width_/height_,1,2048); 
		}
		
		void OnDestroy()
		{
			delete image_[0];
			delete image_[1];
		}

		void OnRender()
		{
			glClearDepth(1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			glEnable(GL_DEPTH_TEST);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glTranslatef(0.0f, -1.0f, -4.0f);
			renderer_.beginRenderPass();
				for(float i = -10; i <= 10; i += 1)
				{
					glBegin(GL_LINES);
						glColor3ub(68, 193, 181);						
						glVertex3f(-10, 0, i);					
						glVertex3f(10, 0, i);
						glVertex3f(i, 0,-10);							
						glVertex3f(i, 0, 10);
					glEnd();
				}
				glColor3f(1, 1, 1);
				glEnable(GL_TEXTURE_2D);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				renderer_.setTexture(GL_TEXTURE0, image_[0]);
				glBegin(GL_QUADS);
					glTexCoord2f(0, 1);
					glVertex3f(-1, 0, 0);
					glTexCoord2f(0, 0);
					glVertex3f(-1, 2, 0);
					glTexCoord2f(1, 0);
					glVertex3f(1, 2, 0);
					glTexCoord2f(1, 1);
					glVertex3f(1, 0, 0);
				glEnd();
			renderer_.endRenderPass();
			glFlush();
		}

		void OnUpdate(float frame_time)
		{
			if( glfwGetKey(GLFW_KEY_ESC) )
				close();
		}
};

int main()
{
	Exest app;
	app.run();
	return 0;
}