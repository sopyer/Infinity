#pragma once

#include <UI.h>
#include <cstdlib>
#include <scheduler.h>

//class Image: public UI::Actor
//{
//	public:
//		Image()  {}
//		~Image() {}
//		
//		Image&	setTexture(GLuint texture) {mTexture = texture; return *this;}
//		GLuint	getTexture() {return mTexture;}
//
//	protected:
//		virtual void onPaint()
//		{
//			float w = getWidth(), h = getHeight(),
//				  x = 0, y = 0;
//			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
//			glUseProgram(0);
//			glEnable(GL_TEXTURE_2D);
//			glBindTexture(GL_TEXTURE_2D, mTexture);
//			glBegin(GL_TRIANGLE_STRIP);
//				glTexCoord2f(0, 0);
//				glVertex2f(x, y);
//				glTexCoord2f(0, 1);
//				glVertex2f(x, y+h);
//				glTexCoord2f(1, 0);
//				glVertex2f(x+w, y);
//				glTexCoord2f(1, 1);
//				glVertex2f(x+w, y+h);
//			glEnd();
//			glDisable(GL_TEXTURE_2D);
//		}
//
//	protected:
//		GLuint		mTexture;
//};
//
//class Rectangle: public UI::Actor
//{
//	protected:
//		virtual void onPaint()
//		{
//			float w = getWidth(), h = getHeight(),
//			    x = 0, y = 0;
//			sui::drawFrame(x, y, w, h, 5, 5, false, false, false);
//		}
//};
//
//class Label: public UI::Actor
//{
//	public:
//		Label(): mColor(1.0f, 1.0f, 1.0f, 1.0f) {}
//		
//		Label& setFont(sui::Font font) {mFont = font; return *this;}
//		Label& setColor(const glm::vec4& color) {mColor = color; return *this;}
//		Label& setText(const wchar_t* text) {mText = text; return *this;}
//
//	protected:
//		virtual void onPaint()
//		{
//			glColor4fv(mColor);
//			float y = sui::getTextAscender(*mFont)+sui::getTextDescender(*mFont);
//			sui::drawText(*mFont, 0, (mHeight+y)/2, mText.c_str());
//		}
//
//	protected:
//		glm::vec4		mColor;
//		sui::Font		mFont;
//		std::wstring	mText;
//};

//class Edit: public Label, public sigslot::has_slots<>
//{
//	public:
//		Edit(): mCaretPos(0), mShowCursor(false), mBlinkTask(0)
//		{
//		}
//	
//		~Edit()
//		{
//			hideCursor();
//		}
//
//	protected:
//		void onBlink()
//		{
//			mShowCursor = !mShowCursor;
//		}
//
//		virtual void onTouch(const ButtonEvent& event/*float x, float y, u32 buttons*/)
//		{
//			getStage()->captureFocus(this);
//		}
//		
//		virtual void onFocusIn()
//		{
//			showCursor();
//		}
//
//		virtual void onFocusOut()
//		{
//			hideCursor();
//		}
//
//		virtual void onKeyDown(const KeyEvent& event/*u32 key*/)
//		{
//			u32 key = event.keysym.sym;
//			// filter for special keys
//			// Enter, quit edition
//			if (key == SDLK_RETURN/*GLFW_KEY_ENTER*/)
//			{
//				return;
//			}
//			// Regular char, append it to the edited string
//			else if (event.keysym.unicode>=32)
//			{
//				std::wstring::iterator	where = mText.end();
//				if (mCaretPos < (int)mText.length())
//				{
//					where = mText.begin()+mCaretPos;
//				}
//				mText.insert(where, event.keysym.unicode);
//				mCaretPos++;
//				resetCursor();
//			}
//			// Special keys
//			else
//			{
//				switch (key)
//				{
//				case SDLK_LEFT:
//					{
//						// move cursor left one char
//						mCaretPos--;
//					} 
//					break;
//				case SDLK_RIGHT:
//					{
//						// move cursor right one char
//						mCaretPos++;
//					} 
//					break;
//				case SDLK_HOME:
//					{
//						mCaretPos = 0;
//					} 
//					break;
//				case SDLK_END:
//					{
//						mCaretPos = (int)mText.length();
//					} 
//					break;
//				case SDLK_INSERT:
//					{
//					} 
//					break;
//				case SDLK_DELETE:
//					{
//						if (mCaretPos < (int)mText.length())
//							mText.erase(mText.begin()+mCaretPos);
//					} 
//					break;
//				case SDLK_BACKSPACE:
//					{
//						if (mCaretPos > 0)
//							mText.erase(mText.begin()+mCaretPos-1);
//						mCaretPos--;
//					} 
//					break;
//				default :
//					{
//						// strange key pressed...
//						//mCaretPos--;
//					} 
//					break;
//				}
//				resetCursor();
//			}	
//			mCaretPos = std::min(mCaretPos, (int)mText.length());
//			mCaretPos = std::max(mCaretPos, 0);
//		}
//
//		virtual void onPaint()
//		{
//			sui::drawRoundedRectOutline(0, 0, mWidth, mHeight, 5, 5, 3);
//			
//			glColor4fv(mColor);
//			float y = (mHeight+sui::getTextAscender(*mFont)+sui::getTextDescender(*mFont))/2;
//			sui::drawText(*mFont, 7, y, mText.c_str());
//
//			if (mShowCursor)
//			{
//				float tw = 7+sui::getTextHExtent(*mFont, mText.substr(0, mCaretPos).c_str());
//				sui::drawRect(tw, y-sui::getTextDescender(*mFont), tw+1, y-sui::getTextAscender(*mFont), 4, 4);
//			}
//		}
//		
//		void showCursor()
//		{
//			mShowCursor = true;
//			mBlinkTask = scheduler::addTimedTask<Edit, &Edit::onBlink>(this, 400);
//		}
//
//		void hideCursor()
//		{
//			mShowCursor = false;
//			if (mBlinkTask)
//			{
//				scheduler::terminateTask(mBlinkTask);
//				mBlinkTask = 0;
//			}
//		}
//
//		void resetCursor()
//		{
//			mShowCursor = true;
//		}
//
//	private:
//		scheduler::Task	mBlinkTask;
//		
//		bool	mShowCursor;
//		int		mCaretPos;
//};
//
//class Button: public Label
//{
//	public:
//		Button(): mIsPressed(false), mIsHover(false) {}
//		
//	public:
//		sigslot::signal0<>	onClicked;
//
//	protected:
//		virtual void onTouch(const ButtonEvent& event/*float x, float y, u32 buttons*/)
//		{mIsPressed = true; onClicked.emit();}
//
//		virtual void onUntouch(const ButtonEvent& event/*float x, float y, u32 buttons*/)
//		{mIsPressed = false;}
//
//		virtual void onEnter()
//		{mIsHover = true;}
//
//		virtual void onLeave()
//		{mIsHover = false; mIsPressed = false;}
//
//		virtual void onPaint()
//		{
//			sui::drawFrame(0, 0, mWidth, mHeight, 5, 5, mIsHover, mIsPressed, false);
//			glColor4fv(mColor);
//			float x = sui::getTextHExtent(*mFont, mText.c_str());
//			float y = sui::getTextAscender(*mFont)+sui::getTextDescender(*mFont);
//			sui::drawText(*mFont, (mWidth-x)/2, (mHeight+y)/2, mText.c_str());
//		}
//
//	protected:
//		bool		mIsHover;
//		bool		mIsPressed;
//};
//
//class CheckBox: public Button
//{
//	public:
//		CheckBox(): mIsChecked(false) {}
//		
//		bool isChecked() {return mIsChecked;}
//	protected:
//		virtual void onUntouch(const ButtonEvent& event/*float x, float y, u32 buttons*/)
//		{Button::onUntouch(event/*x, y, buttons*/); mIsChecked = !mIsChecked;}
//
//		virtual void onPaint()
//		{
//			sui::drawBoolFrame(0, 0, 16, 16, 16/6, 16/6, mIsHover, mIsChecked, false );
//			glColor4fv(mColor);
//			float x = 20;
//			float y = sui::getTextAscender(*mFont)+sui::getTextDescender(*mFont);
//			y = (16+y)/2;
//			sui::drawText(*mFont, x, y, mText.c_str());
//		}
//
//	private:
//		bool		mIsChecked;
//};
//
//class HGroup: public UI::Container
//{
//	public:
//		HGroup()
//		{
//			mTextFont.create("Times 16px", "C:\\WINDOWS\\Fonts\\times.ttf 16px");
//			setSize(100, 30);
//
//			m1.setFont(mTextFont)
//				.setText(L"Menu1")
//				.setSize(100, 20);
//			m2.setFont(mTextFont)
//				.setText(L"Menu2")
//				.setSize(100, 20);
//			m3.setFont(mTextFont)
//				.setText(L"Menu3")
//				.setSize(100, 20);
//
//			add(m1).add(m2).add(m3);
//		}
//
//	protected:
//		void onAllocate()
//		{
//			float x = 0, y = 0;
//			float w = getWidth();
//			ChildsVector::iterator	it = mChilds.begin(),
//									end = mChilds.end();
//			for (; it != end; ++it)
//			{
//				Actor&	actor = *(*it);
//				actor.setPos(0, y)
//					.setWidth(w);
//				y += actor.getHeight();
//			}
//			setSize(w, y);
//		}
//
//	private:
//		FontRef	mTextFont;
//
//		Button	m1;
//		Button	m2;
//		Button	m3;
//};
//
//class Layout: public UI::Container, public sigslot::has_slots<>
//{
//	public:
//		Layout()
//		{
//			mTextFont.create("Times 16px", "C:\\WINDOWS\\Fonts\\times.ttf 16px");
//			setSize(100, 30);
//
//			add
//			(
//				mEdit.setFont(mTextFont)
//					.setText(L"Edit me!!!")
//					.setSize(80, 20)
//					.setPos(0,0)
//			)
//			.
//			add
//			(
//				mButton.setFont(mTextFont)
//					.setText(L"^")
//					.setSize(20, 20)
//					.setPos(80,0)
//			)
//			.
//			add
//			(
//				mListBox.setSize(100, 60)
//					.setPos(0,20)
//					.hide()
//			);
//
//			mButton.onClicked.connect(this, &Layout::expandCollapseList);
//		}
//		
//		void expandCollapseList()
//		{
//			if (mListBox.isVisible())
//			{
//				mListBox.hide();
//			}
//			else
//			{
//				mListBox.show();
//			}
//		}
//
//		void onAllocate()
//		{
//			float w = getWidth();
//			float h = getHeight();
//
//			mButton.setSize(h,h)
//				.setPos(w-h, 0);
//			mEdit.setSize(w-h, h);
//			mListBox.setWidth(w)
//				.setPos(0, h);
//
//			UI::Container::onAllocate();
//		}
//
//	private:
//		FontRef	mTextFont;
//
//		Button	mButton;
//		Edit	mEdit;
//		HGroup	mListBox;
//};
