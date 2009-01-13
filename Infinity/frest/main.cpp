#include <framework.h>
#include "ship_data.h"
#include <FTGLTextureFont.h>
#include "ThirdPersonCamera.h"
#include <UI.h>

class Label: public UI::Actor
{
	public:
		Label(): mColor(1.0f, 1.0f, 1.0f, 1.0f) {}
		
		Label& setFont(FontRef font) {mFont = font; return *this;}
		Label& setColor(const glm::vec4& color) {mColor = color; return *this;}
		Label& setText(const char* text) {mText = text; return *this;}

	protected:
		void onPaint(VG& vg)
		{
			glColor4fv(mColor);
			vg.drawText(*mFont, mPos, mText.c_str());
		}

	protected:
		glm::vec4	mColor;
		FontRef		mFont;
		std::string	mText;
};

class Edit: public Label
{
	public:
		Edit(): mCaretPos(0) {}
	
	protected:
		void onKeyUp(uint32 key)
		{
			// filter for special keys
			// Enter, quit edition
			if (key == GLFW_KEY_ENTER)
			{
				return;
			}
			// Special keys
			else if (key >= GLFW_KEY_SPECIAL)
			{
				switch (key)
				{
				case GLFW_KEY_LEFT :
					{
						// move cursor left one char
						mCaretPos--;
					} 
					break;
				case GLFW_KEY_RIGHT :
					{
						// move cursor right one char
						mCaretPos++;
					} 
					break;
				case GLFW_KEY_HOME :
					{
						mCaretPos = 0;
					} 
					break;
				case GLFW_KEY_END :
					{
						mCaretPos = mText.length();
					} 
					break;
				case GLFW_KEY_INSERT :
					{
					} 
					break;
				case GLFW_KEY_DEL :
					{
						if (mCaretPos < (int)mText.length())
							mText.erase(mText.begin()+mCaretPos);
					} 
					break;
				case GLFW_KEY_BACKSPACE :
					{
						if (mCaretPos > 0)
							mText.erase(mText.begin()+mCaretPos-1);
						mCaretPos--;
					} 
					break;
				default :
					{
						// strange key pressed...
						//mCaretPos--;
					} 
					break;
				}
			}	
			// Regular char, append it to the edited string
			else/* if ( textLength < maxTextLength)*/
			{
				std::string::iterator	where = mText.end();
				if (mCaretPos < (int)mText.length())
				{
					where = mText.begin()+mCaretPos;
				}
				mText.insert(where, (char)key);
				mCaretPos++;
			}
			mCaretPos = std::min(mCaretPos, (int)mText.length());
			mCaretPos = std::max(mCaretPos, 0);
		}

		void onPaint(VG& vg)
		{
			Label::onPaint(vg);
			//Fix me!!!!!: if mCaretPos == 0 the cursor is not drawn
			//Use font metrics for cursor rendering then text bounding box
			glm::ivec2	ex = vg.getTextExtent(*mFont, mText.substr(0, mCaretPos).c_str());
			ex.s += 1;
			vg.drawRect(mColor, mPos+glm::ivec2(ex.s, 0), mPos+glm::ivec2(ex.s, ex.t));
		}

	private:
		int mCaretPos;
};

class Button: public Label
{
	public:
		Button(): mIsPressed(false), mIsHover(false) {}
		
	public:
		sigslot::signal0<>	onClicked;

	protected:
		void onTouch(const glm::ivec2& pos, uint32 buttons)
		{mIsPressed = true;}

		void onUntouch(const glm::ivec2& pos, uint32 buttons)
		{mIsPressed = false; onClicked.emit();}

		void onEnter()
		{mIsHover = true;}

		void onLeave()
		{mIsHover = false; mIsPressed = false;}

		void onPaint(VG& vg)
		{
		    vg.drawFrame(Rect(mPos.x, mPos.y, mSize.s, mSize.t), Point(5, 5), mIsHover, mIsPressed, false);
			glColor4fv(mColor);
			vg.drawText(*mFont, mPos+mSize/2, TextAlign::HCenter | TextAlign::VCenter, mText.c_str());
		}

	protected:
		bool		mIsHover;
		bool		mIsPressed;
};

class CheckBox: public Button
{
	public:
		CheckBox(): mIsChecked(false) {}
		
		bool isChecked() {return mIsChecked;}
	protected:
		void onUntouch(const glm::ivec2& pos, uint32 buttons)
		{Button::onUntouch(pos, buttons); mIsChecked = !mIsChecked;}

		void onPaint(VG& vg)
		{
			///*if (style) */vg.drawFrame( Rect(mPos.x, mPos.y, 16, 16), Point( 5, 5 ), mIsHover, false, false );
			vg.drawBoolFrame(Rect(mPos.x, mPos.y, 16, 16), Point( 16/6, 16/6 ), mIsHover, mIsChecked, false );
		    //vg.drawFrame(Rect(mPos.x, mPos.y, 20, 20), Point(5, 5), mIsHover, mIsChecked, false);
			glColor4fv(mColor);
			vg.drawText(*mFont, mPos+glm::ivec2(20, 8), TextAlign::Left | TextAlign::VCenter, mText.c_str());
		}

	private:
		bool		mIsChecked;
};

class Frest: public Stage, public sigslot::has_slots<>
{
	private:
		VG				mVG;
		gl::Context		mContext;

		FontRef			mTextFont;
		TextureRef		mReflection;
		TextureRef		mHighlight;
		ProgramRef		mProg;

		gl::CommandList	mShip;
		
		GLint	mReflectionLoc;
		GLint	mHighlightLoc;

		float	mPsiDir, mPhiDir;
		bool	mManualRotation;

		ThirdPersonCamera	mCamera;
		float xangle, yangle;
		bool	mShowHelp;

		Button		mButton;
		CheckBox	mCheck;
		Label		mLabel;
		Edit		mEdit;
		UI::Container	mContainer;
	protected:
		void compileShipList();
		virtual void paint();
		
		void onKeyDown(uint32 key)
		{
			switch (key)
			{
				case GLFW_KEY_ESC:
					close();
					break;
				case GLFW_KEY_RIGHT:
					mPhiDir = 1;
					mManualRotation = true;
					//yangle += 50.0f * (mTimer.getFrameTime());
					break;
				case GLFW_KEY_LEFT:
					mPhiDir = -1;
					mManualRotation = true;
					//yangle -= 50.0f * (mTimer.getFrameTime());
					break;
				case GLFW_KEY_UP:
					mPsiDir = -1;
					mManualRotation = true;
					//xangle -= 50.0f * (mTimer.getFrameTime());
					break;
				case GLFW_KEY_DOWN:
					mPsiDir = 1;
					mManualRotation = true;
					//xangle += 50.0f * (mTimer.getFrameTime());
					break;
				case 'H':
					mShowHelp = !mShowHelp;
					break;
			}
		}

		void onKeyUp(uint32 key)
		{
			Stage::onKeyUp(key);
			switch (key)
			{
				case GLFW_KEY_RIGHT:
				case GLFW_KEY_LEFT:
				case GLFW_KEY_UP:
				case GLFW_KEY_DOWN:
					mManualRotation = false;
					break;
			}
		}

		void onUpdate(float timeSinceLastFrame)
		{
			static float lastTime = 0;
			static bool  pressed = false;
			float xangle = 0; yangle = 0;

			bool manual = false;

			/*
			static int look = 0;
			
			if(!look && glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT)) 
			{
				glfwDisable(GLFW_MOUSE_CURSOR);
				glfwSetMousePos(mWidth / 2, mHeight / 2);
				look = 1;
			}
			else if (look && !glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT))
			{
				look = false;
				glfwEnable(GLFW_MOUSE_CURSOR);
			}

			if(look)
			{
				manual = true;
				int mouseX=0, mouseY=0;
				glfwGetMousePos(&mouseX, &mouseY);
				float psi = (mouseX - mWidth / 2)*0.05f;
				float phi = (mouseY - mHeight / 2)*0.05f;
				yangle += psi * (mTimer.getFrameTime());
				xangle += phi * (mTimer.getFrameTime());
				
				glfwSetMousePos(mWidth / 2, mHeight / 2);
			} 
			*/
			if (!mManualRotation)
			{
				xangle = mPhiDir * 0.05f * (mTimer.getFrameTime());
				yangle = mPsiDir * 0.02f * (mTimer.getFrameTime());
			}

			mCamera.rotate(xangle, yangle);
		}

	public:
		Frest():	mVG(mWidth, mHeight),
					mPsiDir(1.0f), mPhiDir(1.0f),
					mManualRotation(false), mShowHelp(true)
		{
			mReflection.create("reflection_texture", "texture1.tga");
			mHighlight.create("highlight_texture", "highlight.tga");

			compileShipList();

			xangle = 0;
			yangle = 0;
			
			mTextFont.create("Times 16px", "C:\\WINDOWS\\Fonts\\times.ttf 16px");

			mProg.create("sph_map_shader", "shader.vert", "shader.frag");
			mReflectionLoc = mProg->bindUniform("reflection");
			mHighlightLoc  = mProg->bindUniform("highlight");
			

			mLabel.setText("Label Test")
				.setFont(mTextFont)
				.setPos(glm::ivec2(0, 0));

			mEdit.setText("Edit me!!!")
				.setFont(mTextFont)
				.setPos(glm::ivec2(0, 40));

			mContainer.add(mLabel)
				.add(mEdit)
				.setPos(glm::ivec2(40, 140));

			mButton.setText("Exit")
				  .setFont(mTextFont)
				  .setPos(glm::ivec2(0, 60))
				  .setSize(glm::ivec2(80, 30));
			
			mCheck.setText("Check")
				  .setFont(mTextFont)
				  .setPos(glm::ivec2(0, 100))
				  .setSize(glm::ivec2(80, 30));

			mButton.onClicked.connect<Frest>(this, &Frest::close);
			add(mButton).add(mCheck).add(mContainer);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(90.0, (float)mWidth/mHeight, 1, 400); 
		}
};

int main()
{
	Frest app;

	app.run();

	return 0;
}

void Frest::paint()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(mCamera.getMatrix());

	mContext.begin();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);

		if (mReflection) mReflection->setWrap(GL_CLAMP, GL_CLAMP);
		if (mHighlight) mHighlight->setWrap(GL_CLAMP, GL_CLAMP);

		mContext.setTexture(GL_TEXTURE0, mReflection.get());
		mContext.setTexture(GL_TEXTURE1, mHighlight.get());
		
		mContext.useProgram(*mProg);
		mContext.setUniform1i(mReflectionLoc, 0);
		mContext.setUniform1i(mHighlightLoc, 1);

		mShip.execute();

		mContext.useProgram(0);

		mContext.setTexture(GL_TEXTURE1, NULL);
		mContext.setTexture(GL_TEXTURE0, NULL);
	mContext.end();
	
	if (mShowHelp)
	{
		mVG.begin();
			//! Temporary hack to change font color
			UI::Container::onPaint(mVG);
			glColor3f(0.3f,1.0f,0.5f);
			mVG.drawText(*mTextFont, 0, 0, "Use cursor keys or mouse to rotate");
			mVG.drawText(*mTextFont, 0, 20, "Press H to toggle help");
			mVG.drawText(*mTextFont, 0, 40, "Press ESC to exit");

		mVG.end();
	}

	glFlush();
}

void Frest::compileShipList()
{
	mShip.begin();
	glBegin(GL_TRIANGLES);

	GLuint ref_count = 0;
	GLuint material_count = material_ref[ref_count][1];
	GLuint color_index = material_ref[ref_count][0];

	glColor3f(materials[color_index].diffuse[0], materials[color_index].diffuse[1], materials[color_index].diffuse[2]); 
	
	for(GLuint i=0; i<8686; i++)
	{
		if(!material_count)
		{
			ref_count++;
			material_count = material_ref[ref_count][1];
			color_index = material_ref[ref_count][0];

			glColor3f(materials[color_index].diffuse[0], materials[color_index].diffuse[1], materials[color_index].diffuse[2]); 
		}

		glNormal3f(normals[face_indicies[i][3]] [0], normals[face_indicies[i][3]] [1], normals[face_indicies[i][3]] [2]);
		glVertex3f(vertices[face_indicies[i][0]] [0], vertices[face_indicies[i][0]] [1], vertices[face_indicies[i][0]] [2]);
		
		glNormal3f(normals[face_indicies[i][4]] [0], normals[face_indicies[i][4]] [1], normals[face_indicies[i][4]] [2]);
		glVertex3f(vertices[face_indicies[i][1]] [0], vertices[face_indicies[i][1]] [1], vertices[face_indicies[i][1]] [2]);
		
		glNormal3f(normals[face_indicies[i][5]] [0], normals[face_indicies[i][5]] [1], normals[face_indicies[i][5]] [2]);
		glVertex3f(vertices[face_indicies[i][2]] [0], vertices[face_indicies[i][2]] [1], vertices[face_indicies[i][2]] [2]);

		material_count--;
	}
	glEnd();
	mShip.end();
}