#include <framework.h>
#include <FTGLTextureFont.h>
#include "Controls.h"

class GUISample: public UI::Stage, public sigslot::has_slots<>
{
	public:
		GUISample():	mVG(mWidth, mHeight), mUpdater(30)
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
			
			mCheck.setText("Check")
				  .setFont(mTextFont)
				  .setPos(0, 100)
				  .setSize(80, 30);

			mButton.onClicked.connect<GUISample>(this, &GUISample::close);
			
			mMenu.setPos(400,20)
				.setSize(200, 100);
			
			mCombobox.setPos(400, 300)
				.setSize(200, 40);
			
			mAvatarImg.create("avatar", "sobachka.png");
			mImage.setTexture(mAvatarImg)
				.setPos(200, 200)
				.setSize(60, 80);

			add(mImage);
			add(mButton);
			add(mCheck);
			add(mContainer);
			add(mMenu);
			add(mCombobox);

			mUpdater.onStarted.connect(this, &GUISample::test);
			mUpdater.onCompleted.connect(this, &GUISample::test);
			mUpdater.start();
		}

	protected:
		void test()
		{
		}

		void paint()
		{
			glClearDepth(1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			mVG.begin();
				UI::Container::onPaint(mVG);
			mVG.end();

			glFlush();
		}

		void onUpdate(uint32 frame)
		{
			//logMessage("Time: %d\n", mTimer.getTime());
			if( glfwGetKey(GLFW_KEY_ESC) )
				close();
		}

	private:
		VG				mVG;
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

		Timeline		mUpdater;
};

int main()
{
	GUISample app;

	app.run();

	return 0;
}