#include <framework.h>
#include <TextureManager.h>
#include <time.h>

class Image: public UI::Actor
{
	public:
		Image()  {}
		~Image() {}
		
		Image&	setTexture(GLuint texture) {mTexture = texture; return *this;}
		GLuint	getTexture() {return mTexture;}

	protected:
		virtual void onPaint()
		{
			float w = getWidth(), h = getHeight(),
				  x = -w/2.0f, y = -h/2.0f;
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glUseProgram(0);
			glAlphaFunc(GL_GREATER, 0.0f);
			glEnable(GL_ALPHA_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, mTexture);
			glBegin(GL_TRIANGLE_STRIP);
				glTexCoord2f(0, 1);
				glVertex2f(x, y);
				glTexCoord2f(0, 0);
				glVertex2f(x, y+h);
				glTexCoord2f(1, 1);
				glVertex2f(x+w, y);
				glTexCoord2f(1, 0);
				glVertex2f(x+w, y+h);
			glEnd();
			glDisable(GL_TEXTURE_2D);
		}

	protected:
		GLuint		mTexture;
};

enum
{
	IMG_BACKGROUND,
	IMG_HOUR_HAND,
	IMG_MINUTE_HAND,
	IMG_SECOND_HAND,
	IMG_COUNT
};

const char*	imageNames[IMG_COUNT] = 
{
	"clockFace.png",
	"hourHand.png",
	"minuteHand.png",
	"secondHand.png",
};

const float w = 10;
const float h = 10;

class ClockSample: public UI::SDLStage
{
	public:
		ClockSample()
		{
			VFS::mount("../AppData");

			impl::acquire();

			glGenTextures(IMG_COUNT, mTextures);
			
			for (int i=0; i<IMG_COUNT; ++i)
			{
				loadTexture(imageNames[i], mTextures[i]);
				mGroup.add(
					mImages[i].setTexture(mTextures[i]).
					setPos(0, 0, -15).
					setSize(w, h)
				);
			}
			
			add(mGroup);

			scheduler::addTimedTask<ClockSample, &ClockSample::timeTick>(this, 200);
		}

		~ClockSample()
		{
			glDeleteTextures(IMG_COUNT, mTextures);
			impl::release();
		}

	protected:
		void timeTick()
		{
			__time64_t	curtime;
			tm		dt;
			_time64(&curtime);
			_gmtime64_s(&dt, &curtime);

			mImages[IMG_SECOND_HAND].setRotation(-6.0f*dt.tm_sec, 0, 0, 1);
			mImages[IMG_MINUTE_HAND].setRotation(-6.0f*dt.tm_min, 0, 0, 1);
			mImages[IMG_HOUR_HAND].setRotation(-15.0f*dt.tm_hour, 0, 0, 1);
		}

	private:
		VFS			mVFS;

		GLuint		mTextures[IMG_COUNT];

		Container	mGroup;
		Image		mImages[IMG_COUNT];
		Image		mBackground;
};

int main(int argc, char** argv)
{
	ui::init();
	{
		ClockSample app;
		app.run();
	}
	ui::cleanup();

	return 0;
}
