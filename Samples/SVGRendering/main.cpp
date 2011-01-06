#include "nanosvg.h"
#include <framework.h>
#include "ResourceHelpers.h"
#include <time.h>
#include <vector>

class SVGSample: public UI::SDLStage
{
	public:
		SVGSample(const char* svgFile):
			mOffsetX(128/*mWidth/2*/),
			mOffsetY(-18/*mHeight/2*/),
			mScale(2.5f),
			mIsDragging(false),
			mDrawTimeCPU(0),
			mDrawTimeGPU(0)
		{
			VFS::mount("../../AppData");
			
			SVGPath* plist;
			plist = svgParseFromFile(svgFile);
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

			mFont = sui::createFont("K:\\media\\Fonts\\AnonymousPro-1.002.001\\Anonymous Pro B.ttf");
			mFont.setSize(16);

			add(mBkgRect.setPos(10, 10)
				  .setSize(300, 100));
			add(mCPUSubmitTimeLabel.setFont(mFont)
				  .setPos(20, 40));
			add(mGPUExecuteTimeLabel.setFont(mFont)
				  .setPos(20, 60));
			add(mAAChoice.setChecked(false)
				  .setText(L"Enable AA")
				  .setFont(mFont)
				  .setPos(20, 80)
				  .setSize(60, 20));
			add(mVideoAdapterLabel.setFont(mFont)
				  .setPos(20, 20));
			
			wchar_t str[256];
			mbstowcs(str, (char*)glGetString(GL_RENDERER), 256);
			mVideoAdapterLabel.setText(str);

			mt::addTimedTask<SVGSample, &SVGSample::onUpdateStats>(this, 100);

			assert(GLEE_EXT_timer_query);

			glGenQueries(1, &mTimeQuery);

			SDL_WM_SetCaption("GPU accelerated SVG rendering", NULL);
		}

		mt::Task		mUpdateTask;
		ui::Rectangle	mBkgRect;
		ui::CheckBox	mAAChoice;
		ui::Label	mVideoAdapterLabel;
		ui::Label	mCPUSubmitTimeLabel;
		ui::Label	mGPUExecuteTimeLabel;
		sui::Font	mFont;
		
		~SVGSample()
		{
			glDeleteQueries(1, &mTimeQuery);

			struct DeletePath
			{void operator ()(vg::Path path) {vg::Path::destroy(path);}};

			for_each(mPaths.begin(), mPaths.end(), DeletePath());

			struct DeletePaint
			{void operator ()(vg::Paint paint) {vg::Paint::destroy(paint);}};

			for_each(mPaints.begin(), mPaints.end(), DeletePaint());

			sui::destroyFont(mFont);
		}

	protected:
		void onUpdateStats()
		{
			wchar_t str[256];
			_snwprintf(str, 256, L"CPU time - %f ms", mDrawTimeCPU);
			mCPUSubmitTimeLabel.setText(str);
			_snwprintf(str, 256, L"GPU time - %f ms", mDrawTimeGPU);
			mGPUExecuteTimeLabel.setText(str);
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
			CPUTimer drawTimer;
			drawTimer.start();

			glBeginQuery(GL_TIME_ELAPSED_EXT, mTimeQuery);
			
			glClearColor(1, 1, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			glMatrixMode(GL_MODELVIEW);
			glTranslatef(mOffsetX, mOffsetY, 0);
			glScalef(mScale, mScale, 1);

			if (mAAChoice.isChecked())
			{
				for (size_t i=mPaths.size(); i!=0; i--)
					vg::drawPathNZA2C(mPaths[i-1], mPaints[i-1]);
			}
			else
			{
				glDisable(GL_MULTISAMPLE);
				for (size_t i=mPaths.size(); i!=0; i--)
					vg::drawPathNZ(mPaths[i-1], mPaints[i-1]);
				glEnable(GL_MULTISAMPLE);
			}

			glEndQuery(GL_TIME_ELAPSED_EXT);

			mDrawTimeCPU = 0.8f*drawTimer.elapsed()+0.2f*mDrawTimeCPU;

			GLuint64EXT result;
			glGetQueryObjectui64vEXT(mTimeQuery, GL_QUERY_RESULT, &result);
			mDrawTimeGPU = 0.8f*result/1000.0f/1000.0f+0.2f*mDrawTimeGPU;
		}

	private:
		GLuint		mTimeQuery;
		VFS			mVFS;
		bool		mIsDragging;
		float		mOffsetX;
		float		mOffsetY;
		float		mScale;

		float		mDrawTimeCPU;
		float		mDrawTimeGPU;

		std::vector<vg::Path>	mPaths;
		std::vector<vg::Paint>	mPaints;	
};

int main(int argc, char** argv)
{
	ui::init();
	{
		SVGSample app(argc==2?argv[1]:"butterfly.svg");
		app.run();
	}
	ui::cleanup();

	return 0;
}
