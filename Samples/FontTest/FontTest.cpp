#include <framework.h>
#include "FTGLTextureFont.h"
#include <assert.h>

class FontTest: public ui::SDLStage
{
	FTGLTextureFont* font;
	public:
		FontTest()
		{
			font = new FTGLTextureFont("C:\\Windows\\Fonts\\times.ttf");
			font->FaceSize(20);
		}
		
		~FontTest()
		{
			delete font;
		}

	protected:
		virtual void onPaint()
		{
			glMatrixMode(GL_MODELVIEW);
			glTranslatef(100, 100, 0);
			glScalef(1, -1, 1);
			glColor3f(1, 1, 1);
			font->Render("Hello World!!!");
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
	ui::init();
	{
		FontTest app;
		app.run();
	}
	ui::cleanup();

	return 0;
}