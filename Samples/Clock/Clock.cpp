#include <framework.h>
#include <time.h>

#include "ResourceHelpers.h"

#define RASTER_ACTORS

#ifdef RASTER_ACTORS
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
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glUseProgram(0);
			glAlphaFunc(GL_GREATER, 0.0f);
			glEnable(GL_ALPHA_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, mTexture);
			glBegin(GL_TRIANGLE_STRIP);
				glTexCoord2f(0, 0);
				glVertex2f(x, y);
				glTexCoord2f(0, 1);
				glVertex2f(x, y+h);
				glTexCoord2f(1, 0);
				glVertex2f(x+w, y);
				glTexCoord2f(1, 1);
				glVertex2f(x+w, y+h);
			glEnd();
			glDisable(GL_TEXTURE_2D);
			glPopAttrib();
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

const float w = 540;
const float h = 540;

#else

#	include "ClockData.h"

class VectorImage: public UI::Actor
{
	public:
		VectorImage()  {}
		~VectorImage() {}
		
		VectorImage&	setPaint(vg::Paint paint) {mPaint = paint; return *this;}
		VectorImage&	setPath(vg::Path path) {mPath = path; return *this;}

	protected:
		virtual void onPaint()
		{
			vg::drawPathA2C(mPath, mPaint);
		}

	protected:
		vg::Paint		mPaint;
		vg::Path		mPath;
};
#endif

class ClockSample: public UI::SDLStage
{
	public:
		ClockSample():
			mOffsetX(mWidth/2),
			mOffsetY(mHeight/2),
#ifndef	RASTER_ACTORS
			mScale(7),
#else
			mScale(1),
#endif
			mIsDragging(false)
		{
			VFS::mount("../../AppData");
			
#ifdef	RASTER_ACTORS
			glGenTextures(IMG_COUNT, mTextures);
			
			for (int i=0; i<IMG_COUNT; ++i)
			{
				mTextures[i] = resources::createTexture2D(imageNames[i]);
				mGroup.add(
					mImages[i].setTexture(mTextures[i]).
					setSize(w, h)
				);
			}
#else			
			mRed = vg::Paint::createSolid(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
			mBlack = vg::Paint::createSolid(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
			mWhite = vg::Paint::createSolid(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			
			loadClockFacePaths();
			loadClockHands();
#endif

			updateClock();
			add(mGroup);

			mt::addTimedTask<ClockSample, &ClockSample::timeTick>(this, 200);
		}

		~ClockSample()
		{
#ifdef	RASTER_ACTORS
			glDeleteTextures(IMG_COUNT, mTextures);
#else
			for (size_t i=0; i<PATH_COUNT; ++i)
				vg::Path::destroy(mClockPaths[i]);

			vg::Paint::destroy(mWhite);
			vg::Paint::destroy(mBlack);
			vg::Paint::destroy(mRed);
#endif
		}

	protected:
#ifndef RASTER_ACTORS
		void loadClockFacePaths()
		{
			float w, h, x, y;
			float x1, y1, x2, y2;

			for (size_t i=PATH_CLOCK_BKG; i<PATH_HOUR_HAND; ++i)
			{
				mClockPaths[i]  = vg::Path::create(clockVectorImages[i].segCount,
					clockVectorImages[i].segs, clockVectorImages[i].data);

				mClockPaths[i].getBounds(x1, y1, x2, y2);
				
				w = x2-x1; h = y2-y1;
				x = (-w/2.0f-x1); y = (-h/2.0f-y1);

				mGroup.add(mClockActors[i].setPath(mClockPaths[i]).
					setPaint(i==PATH_CLOCK_BKG?mWhite:mBlack).
					setPos(x, y)
					);
			}
		}

		void loadClockHands()
		{
			for (size_t i=PATH_HOUR_HAND; i<PATH_COUNT; ++i)
			{
				mClockPaths[i]  = vg::Path::create(clockVectorImages[i].segCount,
					clockVectorImages[i].segs, clockVectorImages[i].data);

				mGroup.add(mClockActors[i].setPath(mClockPaths[i]).
					setPaint(i==PATH_SECOND_HAND?mRed:mBlack)
					);
			}
		}
#endif

		void timeTick()
		{
			__time64_t	curtime;
			tm		dt;
			_time64(&curtime);
			_gmtime64_s(&dt, &curtime);

#ifdef RASTER_ACTORS
			mImages[IMG_SECOND_HAND].setRotation(6.0f*dt.tm_sec, 0, 0, 1);
			mImages[IMG_MINUTE_HAND].setRotation(6.0f*dt.tm_min, 0, 0, 1);
			mImages[IMG_HOUR_HAND].setRotation(30.0f*(dt.tm_hour%12), 0, 0, 1);
#else
			mClockActors[PATH_SECOND_HAND].setRotation(6.0f*dt.tm_sec-90, 0, 0, 1);
			mClockActors[PATH_MINUTE_HAND].setRotation(6.0f*dt.tm_min-90, 0, 0, 1);
			mClockActors[PATH_HOUR_HAND].setRotation(30.0f*(dt.tm_hour%12)-90, 0, 0, 1);
#endif
		}

		void onTouch(const ButtonEvent& event)
		{
			if (event.button == SDL_BUTTON_WHEELUP)
			{
				mScale *= 1.2f;
				mOffsetX -= (event.x-mOffsetX)*(1.2f - 1);
				mOffsetY -= (event.y-mOffsetY)*(1.2f - 1);
			}
			else if (event.button == SDL_BUTTON_WHEELDOWN)
			{
				mScale /= 1.2f;
				if (mScale<1.0f)
				{
					//fix it a bit
					mOffsetX -= (event.x-mOffsetX)*(1/mScale/1.2f - 1);
					mOffsetY -= (event.y-mOffsetY)*(1/mScale/1.2f - 1);
					mScale = 1.0f;
				}
				else
				{
					mOffsetX -= (event.x-mOffsetX)*(1/1.2f - 1);
					mOffsetY -= (event.y-mOffsetY)*(1/1.2f - 1);
				}
			}
			else
				mIsDragging = true;

			updateClock();
		}

		void onUntouch(const ButtonEvent& event)
		{
			mIsDragging = false;
			updateClock();
		}

		void onMotion(const MotionEvent& event)
		{
			if (mIsDragging)
			{
				mOffsetX += event.xrel;
				mOffsetY += event.yrel;
			}
			updateClock();
		}
		
		void updateClock()
		{
			mGroup.setScale(mScale, mScale).setPos(mOffsetX, mOffsetY);
		}

	private:
		VFS			mVFS;
		bool		mIsDragging;
		float		mOffsetX;
		float		mOffsetY;
		float		mScale;

#ifdef	RASTER_ACTORS
		GLuint		mTextures[IMG_COUNT];

		Image		mImages[IMG_COUNT];
		Image		mBackground;
#else
		vg::Path	mClockPaths[PATH_COUNT];
		
		vg::Paint	mWhite;
		vg::Paint	mRed;
		vg::Paint	mBlack;

		VectorImage	mClockActors[PATH_COUNT];
#endif

		Container	mGroup;
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
