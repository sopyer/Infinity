#include <framework.h>
#include "FTGLTextureFont.h"
#include <assert.h>

class FontTest: public ui::SDLStage
{
	Font font;
	public:
		FontTest()
		{
			font = createFont("C:\\Windows\\Fonts\\times.ttf", 20);
		}
		
		~FontTest()
		{
			destroyFont(font);
		}

	protected:
		virtual void onPaint()
		{
			glMatrixMode(GL_MODELVIEW);
			glTranslatef(100, 100, 0);
			glScalef(1, -1, 1);
			glColor3f(1, 1, 1);
			drawString(font, 0, 0, "Hello World!!!");

			float xmin, ymin, xmax, ymax;
			getBounds(font, "Hello World!!!", xmin, ymin, xmax, ymax);
			glColor3f(1, 0, 0);
			glBegin(GL_LINE_STRIP);
				glVertex2f(xmin,  ymin);
				glVertex2f(xmin,  ymax);
				glVertex2f(xmax,  ymax);
				glVertex2f(xmax,  ymin);
				glVertex2f(xmin,  ymin);
			glEnd();


			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glTranslatef(0, 0, -10);
			
			glColor3f(1, 1, 0);
			glBegin(GL_QUADS);
				glVertex2f(-1,  1);
				glVertex2f( 1,  1);
				glVertex2f( 1, -1);
				glVertex2f(-1, -1);
			glEnd();
		}

	private:
		VFS				mVFS;
};

extern "C" int main(int argc, char** argv)
{
	FontTest app;
	app.run();

	return 0;
}