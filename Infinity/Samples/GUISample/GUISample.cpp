#include <framework.h>
#include <FTGLTextureFont.h>
#include "Controls.h"
#include "Animations.h"

class GUISample: public UI::Stage
{
	public:
		GUISample():mUpdater(30), mAnim(2000, 100)
		{
			mUpdater.onFrame.connect(this, &GUISample::onUpdate);

			mTextFont.create("Times 16px", "C:\\WINDOWS\\Fonts\\times.ttf 16px");

			mLabel.setText("Label Test")
				.setFont(mTextFont)
				.setPos(0, 0);

			mEdit.setText("Edit me!!!")
				.setFont(mTextFont)
				.setPos(0, 40)
				.setSize(100, 30);
			
			captureFocus(&mEdit);

			mContainer.add(mLabel)
				.add(mEdit)
				.setPos(40, 140);

			mButton.setText("Exit")
				  .setFont(mTextFont)
				  .setPos(0, 60)
				  .setSize(80, 30);
			
			mMoveDog.setText("Move dog;)")
				  .setFont(mTextFont)
				  .setPos(100, 60)
				  .setSize(120, 30);

			mCheck.setText("Check")
				  .setFont(mTextFont)
				  .setPos(0, 100)
				  .setSize(80, 30);

			mButton.onClicked.connect<GUISample>(this, &GUISample::close);
			
			mMenu.setPos(400,20)
				.setSize(200, 100)
				.setZ(300)
				.setScaleX(2)
				.setScaleY(2)
				.setRotation(-60, 0, 1, 0);
			
			mCombobox.setPos(400, 300)
				.setSize(200, 40);
			
			mAvatarImg.create("sobachka.png");
			mImage.setTexture(mAvatarImg)
				.setPos(200, 200)
				.setSize(60, 80);

			add(mImage);
			add(mButton);
			add(mMoveDog);
			add(mCheck);
			add(mContainer);
			add(mMenu);
			add(mCombobox);

			mUpdater.onStarted.connect(this, &GUISample::test);
			mUpdater.onCompleted.connect(this, &GUISample::test);
			mUpdater.start();

			mMover.setOffset(300, 0)
				.setTimeline(&mAnim)
				.setInterpolator(&mItrp)
				.apply(&mImage);

			mMoveDog.onClicked.connect(this, &GUISample::onMoveDog);

		}

	protected:
		void test()
		{
		}

		void paint()
		{
			//glClearDepth(1.0);
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//
			//mVG.begin();
			//	UI::Container::onPaint(mVG);
			//mVG.end();

			//glFlush();
		}

		void onUpdate(u32 frame)
		{
			//logMessage("Time: %d\n", mTimer.getTime());
			if( glfwGetKey(KeySyms::KeyESC/*GLFW_KEY_ESC*/) )
				close();
		}

		void onMoveDog()
		{
			mAnim.stop();
			mAnim.start();
		}

	private:
		gl::Context		mContext;

		FontRef			mTextFont;
		TextureRef		mAvatarImg;

		Button			mButton;
		CheckBox		mCheck;
		Label			mLabel;
		Edit			mEdit;
		UI::Container	mContainer;
		HGroup			mMenu;
		Layout			mCombobox;
		Image			mImage;
		Button			mMoveDog;

		Timeline		mUpdater;
		Timeline		mAnim;
		Interpolator	mItrp;
		OffsetAnimation	mMover;
};

int main()
{
	GUISample app;

	app.run();

	return 0;
}