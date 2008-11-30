#include <framework.h>
#include <FTGLTextureFont.h>

class FontApp: public Framework
{
	private:
		FTFont*	times;
		glTexture*	tex;
	protected:
		void makeOrtho()
		{
			int w = width_;
			int h = height_;
			GLdouble aspect = (GLdouble)w / (GLdouble)h;
			// Use the whole window.
			glViewport(0, 0, w, h);
			// We are going to do some 2-D orthographic drawing.
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
			// Now determine where to draw things.
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
		}

		void OnCreate()
		{
			times = new FTGLTextureFont("C:\\WINDOWS\\Fonts\\times.ttf");
			times->FaceSize(24);
			makeOrtho();
			//glMatrixMode(GL_PROJECTION);
			//glLoadIdentity();
			//gluPerspective(90.0, (float)width_/height_,1,2048); 
			//glOrtho(0, width_, 0, height_, -1, 1);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_DEPTH_TEST);
			//int df;
			//glGetIntegerv(GL_DEPTH_FUNC, &df);
			//assert(df==GL_LESS);
			//glDepthFunc(GL_LEQUAL);
			tex = loadJpegTexture("Fieldstone.jpg");
			//glEnable(GL_TEXTURE_2D);
			//glEnable(GL_BLEND);
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glClearDepth(1.0);
			glClearColor(0,0,0,0);
		}
		
		void OnDestroy()
		{
			delete times;
			delete tex;
		}

		void OnRender()
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glColor3f(1,1,1);
			//glLoadIdentity();
			glPushMatrix();
			{
				glEnable(GL_TEXTURE_2D);
				//glEnable(GL_BLEND);
				//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				//glTranslatef(-width_/2, height_/2, 0.0);
				float x1, y1, z1, x2, y2, z2;
				/*FTBBox bb = */times->BBox("Hello world", x1, y1, z1, x2, y2, z2);
				glm::vec2 low(x1, y1), up(x2, y2), pos(0,0);
				pos = -(up-low)/2.0f+pos;
				glTranslatef(pos.x, pos.y, 0);
				times->Render("Hello world");
				glDisable(GL_TEXTURE_2D);
				//glDisable(GL_BLEND);
			}
			glPopMatrix();
			
			//glEnable(GL_BLEND);
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			//glBegin(GL_TRIANGLES);
			//	glColor3f(0.5f, 0.8f, 0.4f);
			//	//glTexCoord2f(0, 0);
			//	glVertex3f(50, 0, 0);
			//	glColor3f(0.2f, 0.8f, 0.3f);
			//	//glTexCoord2f(0.5, 0);
			//	glVertex3f(25, -50, 0);
			//	glColor3f(0.5f, 0.3f, 0.4f);
			//	//glTexCoord2f(1, 1);
			//	glVertex3f(0, 0, 0);
			//glEnd();
			//glEnable(GL_TEXTURE_2D);
			//glBindTexture(GL_TEXTURE_2D, tex->handle_);
			//glBegin(GL_QUADS);
			//	glTexCoord2f(0,0);
			//	glVertex2f(-120, 120);
			//	glTexCoord2f(1,0);
			//	glVertex2f(0, 120);
			//	glTexCoord2f(1,1);
			//	glVertex2f(0, 0);
			//	glTexCoord2f(0,1);
			//	glVertex2f(-120, 0);
			//glEnd();
			//glDisable(GL_TEXTURE_2D);
			//glDisable(GL_TEXTURE_2D);
			//glDisable(GL_BLEND);
			
			glFlush();
			//glfwSleep(0.6);
		}

		void OnUpdate(float frame_time)
		{
			if( glfwGetKey(GLFW_KEY_ESC) )
				close();
		}
};

int main()
{
	FontApp app;
	app.run();
	return 0;
}