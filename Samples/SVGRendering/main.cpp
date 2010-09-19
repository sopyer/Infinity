#include "nanosvg.h"
#include <framework.h>
#include "ResourceHelpers.h"
#include <time.h>
#include <vector>

class SVGSample: public UI::SDLStage
{
	public:
		SVGSample():
			mOffsetX(mWidth/2),
			mOffsetY(mHeight/2),
			mScale(1),
			mIsDragging(false)
		{
			VFS::mount("../../AppData");
			
			SVGPath* plist;
			plist = svgParseFromFile("butterfly.svg");
			SVGPath* cur = plist;
			
			while (cur && cur->hasFill)
			{
				vg::Path path = vg::Path::create(cur->count, cur->commands, cur->data);
				mPaths.push_back(path);

				unsigned int col = 0xFFFFFFFF;
				vg::Paint paint = vg::Paint::createSolid(cur->fillColor);
				mPaints.push_back(paint);

				cur = cur->next;
			}

			svgDelete(plist);
		}

		~SVGSample()
		{
			struct DeletePath
			{void operator ()(vg::Path path) {vg::Path::destroy(path);}};

			for_each(mPaths.begin(), mPaths.end(), DeletePath());

			struct DeletePaint
			{void operator ()(vg::Paint paint) {vg::Paint::destroy(paint);}};

			for_each(mPaints.begin(), mPaints.end(), DeletePaint());
		}

	protected:
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
		}

		void onUntouch(const ButtonEvent& event)
		{
			mIsDragging = false;
		}

		void onMotion(const MotionEvent& event)
		{
			if (mIsDragging)
			{
				mOffsetX += event.xrel;
				mOffsetY += event.yrel;
			}
		}
		
		void onPaint()
		{
			glClearColor(1, 1, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			glMatrixMode(GL_MODELVIEW);
			glTranslatef(mOffsetX, mOffsetY, 0);
			glScalef(mScale, mScale, 1);

			for (size_t i=mPaths.size(); i!=0; i--)
				vg::drawPathAA(mPaths[i-1], mPaints[i-1]);
			//glColor3f(1, 0, 0);
			//glBegin(GL_QUADS);
			//glVertex2f(-100, 100);
			//glVertex2f(100, 100);
			//glVertex2f(100, -100);
			//glVertex2f(-100, 100);
			//glEnd();
		}

	private:
		VFS			mVFS;
		bool		mIsDragging;
		float		mOffsetX;
		float		mOffsetY;
		float		mScale;

		std::vector<vg::Path>	mPaths;
		std::vector<vg::Paint>	mPaints;	
};

int main(int argc, char** argv)
{
	ui::init();
	{
		SVGSample app;
		app.run();
	}
	ui::cleanup();

	return 0;
}
