#include <TextureManager.h>
#include <ProgramManager.h>
#include <ShaderManager.h>
#include <FontManager.h>
#include <framework.h>
#include <FTGLTextureFont.h>
#include "Controls.h"
#include "Animations.h"
#include <VG\Context.h>

class LabelTest: public UI::Actor
{
	public:
		LabelTest(): mColor(1.0f, 1.0f, 1.0f, 1.0f) {}
		
		LabelTest& setFont(vg::Font font) {mFont = font; return *this;}
		LabelTest& setColor(const glm::vec4& color) {mColor = color; return *this;}
		LabelTest& setText(const char* text) {mText = text; return *this;}

	protected:
		virtual void onPaint(VG& vg)
		{
			glColor4fv(mColor);
			vg::drawString(mFont, 0, 0, "TestString");
		}

	protected:
		glm::vec4	mColor;
		vg::Font	mFont;
		std::string	mText;
};

class GUISample: public UI::SDLStage
{
	public:
		GUISample()/*:mUpdater(30), mAnim(2000, 100)*/
		{
			VFS::mount("..\\AppData");

			//mUpdater.onFrame.connect(this, &GUISample::onUpdate);

			mTextFont.create("Times 16px", "C:\\WINDOWS\\Fonts\\times.ttf 16px");
			
			mFont = vg::createFont("C:\\WINDOWS\\Fonts\\times.ttf");
			mFont.setFaceSize(16);

			mLabel.setText(L"Label Test")
				.setFont(mTextFont)
				.setPos(0, 0);

			mEdit.setText(L"Edit me!!!")
				.setFont(mTextFont)
				.setPos(0, 40)
				.setSize(100, 30);
			
			captureFocus(&mEdit);

			mContainer.add(mLabel)
				.add(mEdit)
				.setPos(40, 140);

			mButton.setText(L"Exit")
				  .setFont(mTextFont)
				  .setPos(0, 60)
				  .setSize(80, 30);
			
			mMoveDog.setText(L"Move dog;)")
				  .setFont(mTextFont)
				  .setPos(100, 60)
				  .setSize(120, 30);

			mCheck.setText(L"Check")
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
			
			glGenTextures(1, &mTexture);
			mImage.setTexture(mTexture)
				  .setPos(200, 200)
				  .setSize(60, 80);
			loadTexture("sobachka.png", mTexture);

			add(mImage);
			add(mButton);
			add(mMoveDog);
			add(mCheck);
			add(mContainer);
			add(mMenu);
			add(mCombobox);
			add(mTestLabel.setFont(mFont)
				.setPos(0, 500)
			);

			//mUpdater.onStarted.connect(this, &GUISample::test);
			//mUpdater.onCompleted.connect(this, &GUISample::test);
			//mUpdater.start();

			//mMover.setOffset(300, 0)
			//	.setTimeline(&mAnim)
			//	.setInterpolator(&mItrp)
			//	.apply(&mImage);

			mMoveDog.onClicked.connect(this, &GUISample::onMoveDog);


			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadMatrixf(glm::perspectiveGTX(60.0f, mWidth/mHeight, 0.1f, 600.0f));
			
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
		}
		
		~GUISample()
		{
			vg::destroyFont(mFont);
			glDeleteTextures(1, &mTexture);
		}
	protected:
		void onMoveDog()
		{
			//mAnim.stop();
			//mAnim.start();
		}

	private:
		VFS				mVFS;
		FontManager		mFontMgr;
		ProgramManager	mProgramMgr;
		ShaderManager	mShaderMgr;

		vg::Font		mFont;
		vg::Context		mContext;

		GLuint			mTexture;
		FontRef			mTextFont;

		Button			mButton;
		CheckBox		mCheck;
		Label			mLabel;
		Edit			mEdit;
		UI::Container	mContainer;
		HGroup			mMenu;
		Layout			mCombobox;
		Image			mImage;
		Button			mMoveDog;
		LabelTest		mTestLabel;

		//Timeline		mUpdater;
		//Timeline		mAnim;
		Interpolator	mItrp;
		//OffsetAnimation	mMover;
};

extern "C" int main(int argc, char** argv)
{
	ui::init();
	{
		GUISample app;
		app.run();
	}
	ui::cleanup();

	return 0;
}