#pragma once

#include <UI.h>

class Image: public UI::Actor
{
	public:
		Image() {}
		
		Image& setTexture(TextureRef texture) {mTexture = texture; return *this;}

	protected:
		void onPaint(VG& vg)
		{
			float w = getWidth(), h = getHeight(),
				  x = getX(), y = getY();
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glEnable(GL_TEXTURE_2D);
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
		}

	protected:
		TextureRef		mTexture;
};

class Rectangle: public UI::Actor
{
	protected:
		void onPaint(VG& vg)
		{
			uint32 w = getWidth(), h = getHeight(),
				   x = getX(), y = getY();
		    vg.drawFrame(Rect(x, y, w, h), Point(5, 5), false, false, false);
		}
};

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
			vg.drawText(*mFont, mX, mY, mText.c_str());
		}

	protected:
		glm::vec4	mColor;
		FontRef		mFont;
		std::string	mText;
};

class Edit: public Label, public sigslot::has_slots<>
{
	public:
		Edit(): mCaretPos(0), mShowCursor(false), mBlinkTimeline(3)
		{
			mBlinkTimeline.onFrame.connect(this, &Edit::onBlink);
		}
	
	protected:
		void onBlink(uint32)
		{
			mShowCursor = !mShowCursor;
		}

		void onTouch(float x, float y, uint32 buttons)
		{
			getStage()->captureFocus(this);
		}
		
		void onFocusIn()
		{
			mShowCursor = true;
			mBlinkTimeline.start();
		}

		void onFocusOut()
		{
			mShowCursor = false;
			mBlinkTimeline.stop();
		}

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
						mCaretPos = (int)mText.length();
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
			if (mShowCursor)
			{
				glm::ivec2	pos(mX, mY);
				glm::ivec2	ex = vg.getTextExtent(*mFont, mText.substr(0, mCaretPos).c_str());
				ex.s += 1;
				vg.drawRect(mColor, pos+glm::ivec2(ex.s, mFont->Descender()/*0*/), pos+glm::ivec2(ex.s, mFont->Ascender()/*ex.t*/));
			}
		}

	private:
		Timeline	mBlinkTimeline;
		
		bool	mShowCursor;
		int		mCaretPos;
};

class Button: public Label
{
	public:
		Button(): mIsPressed(false), mIsHover(false) {}
		
	public:
		sigslot::signal0<>	onClicked;

	protected:
		void onTouch(float x, float y, uint32 buttons)
		{mIsPressed = true;}

		void onUntouch(float x, float y, uint32 buttons)
		{mIsPressed = false; onClicked.emit();}

		void onEnter()
		{mIsHover = true;}

		void onLeave()
		{mIsHover = false; mIsPressed = false;}

		void onPaint(VG& vg)
		{
		    vg.drawFrame(Rect(mX, mY, mWidth, mHeight), Point(5, 5), mIsHover, mIsPressed, false);
			glColor4fv(mColor);
			glm::ivec2	mPos(mX, mY), mSize(mWidth, mHeight);
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
		void onUntouch(float x, float y, uint32 buttons)
		{Button::onUntouch(x, y, buttons); mIsChecked = !mIsChecked;}

		void onPaint(VG& vg)
		{
			///*if (style) */vg.drawFrame( Rect(mPos.x, mPos.y, 16, 16), Point( 5, 5 ), mIsHover, false, false );
			vg.drawBoolFrame(Rect(mX, mY, 16, 16), Point( 16/6, 16/6 ), mIsHover, mIsChecked, false );
		    //vg.drawFrame(Rect(mPos.x, mPos.y, 20, 20), Point(5, 5), mIsHover, mIsChecked, false);
			glColor4fv(mColor);
			vg.drawText(*mFont, mX+20, mY+8, TextAlign::Left | TextAlign::VCenter, mText.c_str());
		}

	private:
		bool		mIsChecked;
};

class HGroup: public UI::Container
{
	public:
		HGroup()
		{
			mTextFont.create("Times 16px", "C:\\WINDOWS\\Fonts\\times.ttf 16px");
			setSize(100, 30);

			m1.setFont(mTextFont)
				.setText("Menu1")
				.setSize(100, 20);
			m2.setFont(mTextFont)
				.setText("Menu2")
				.setSize(100, 20);
			m3.setFont(mTextFont)
				.setText("Menu3")
				.setSize(100, 20);

			add(m1).add(m2).add(m3);
		}

	protected:
		void onAllocate()
		{
			int x = 0, y = 0;
			int w = getWidth();
			ChildsVector::iterator	it = mChilds.begin(),
									end = mChilds.end();
			for (; it != end; ++it)
			{
				Actor&	actor = *(*it);
				actor.setPos(0, y)
					.setWidth(w);
				y += actor.getHeight();
			}
			setSize(w, y);
		}

	private:
		FontRef	mTextFont;

		Button	m1;
		Button	m2;
		Button	m3;
};

class Layout: public UI::Container, public sigslot::has_slots<>
{
	public:
		Layout()
		{
			mTextFont.create("Times 16px", "C:\\WINDOWS\\Fonts\\times.ttf 16px");
			setSize(100, 30);

			add
			(
				mEdit.setFont(mTextFont)
					.setText("Edit me!!!")
					.setSize(80, 20)
					.setPos(0,0)
			)
			.
			add
			(
				mButton.setFont(mTextFont)
					.setText("^")
					.setSize(20, 20)
					.setPos(80,0)
			)
			.
			add
			(
				mListBox.setSize(100, 60)
					.setPos(0,20)
					.hide()
			);

			mButton.onClicked.connect(this, &Layout::expandCollapseList);
		}
		
		void expandCollapseList()
		{
			if (mListBox.isVisible())
			{
				mListBox.hide();
			}
			else
			{
				mListBox.show();
			}
		}

		void onAllocate()
		{
			int w = getWidth();
			int h = getHeight();

			mButton.setSize(h,h)
				.setPos(w-h, 0);
			mEdit.setSize(w-h, h);
			mListBox.setWidth(w)
				.setPos(0, h);

			UI::Container::onAllocate();
		}

	private:
		FontRef	mTextFont;

		Button	mButton;
		Edit	mEdit;
		HGroup	mListBox;
};
