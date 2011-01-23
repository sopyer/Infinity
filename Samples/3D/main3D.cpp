#include <framework.h>

class Rectangle1: public ui::Actor
{
	protected:
		virtual void onPaint()
		{
			float	w = getWidth(), h = getHeight(),
					x = 0, y = 0;

			glColor3f(0.9f, 0.45f, 0.67f);
			glBegin(GL_QUADS);
			glVertex2f(x,   y);
			glVertex2f(x,   y+h);
			glVertex2f(x+w, y+h);
			glVertex2f(x+w, y);
			glEnd();
		}
};

const int numRows = 4;
const int numCols = 20;
const float hMargin = 0.15f;
const float vMargin = 0.15f;

class Sample3D: public ui::SDLStage
{
	public:
		Sample3D():doMove(false), offsetX(0), offsetY(0)
		{
			impl::acquire();

			for(int i=0; i<numRows; ++i)
			{
				for(int j=0; j<numCols; ++j)
				{
					mGroup.add(
						mRect[i*numCols+j].setPos((4+hMargin*2.0f)*(j-numCols/2.0f)+hMargin, (3+vMargin*2.0f)*(i-numRows/2.0f)+vMargin)
						.setZ(15)
						.setSize(4, 3)
					);
				}
			}

			add(mGroup);
		}

		~Sample3D()
		{
			impl::release();
		}

	protected:
		bool doMove;
		float offsetX;
		float offsetY;

		void onTouch(const ButtonEvent& event)
		{
			doMove = true;
		}

		void onUntouch(const ButtonEvent& event)
		{
			doMove = false;
		}

		void onMotion(const MotionEvent& event)
		{
			if (doMove)
			{
				offsetX += event.xrel/70.0f;
				//offsetY += event.yrel/100.0f;
			}
			mGroup.setPos(offsetX, 0/*offsetY*/);
		}

		void drawQuad(const glm::vec2& min, const glm::vec2& max, float offset)
		{
			glBegin(GL_QUADS);
			glVertex2f(min.x-offset, min.y-offset);
			glVertex2f(min.x-offset, max.y+offset);
			glVertex2f(max.x+offset, max.y+offset);
			glVertex2f(max.x+offset, min.y-offset);
			glEnd();
		}

		void onPaint()
		{
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();

			glColor3f(1.0f, 1.0f, 1.0f);
			glTranslatef(0, 0, -10);
			glScalef(0.5, 0.5, 1);

			drawQuad(glm::vec2(-1, -1), glm::vec2(1, 1), 0);

			glPopMatrix();
			glPopAttrib();
		}

	private:
		Container	mGroup;
		Rectangle1	mRect[numRows*numCols];//Rectangle	mRect;
};

int main(int argc, char** argv)
{
	ui::init();
	{
		Sample3D app;
		app.run();
	}
	ui::cleanup();

	return 0;
}
