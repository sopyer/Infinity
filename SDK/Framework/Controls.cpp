#include "Controls.h"

#pragma once

#include <UI.h>
#include <cstdlib>
#include <scheduler.h>
#include <VG/VG.h>

namespace ui
{
	void Image::onPaint()
	{
		float w = getWidth(), h = getHeight(),
				x = 0, y = 0;
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glUseProgram(0);
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

	void Rectangle::onPaint()
	{
		float w = getWidth(), h = getHeight(),
			x = 0, y = 0;
		sui::drawFrame(x, y, w, h, 5, 5, false, false, false);
	}

	void Label::onPaint()
	{
		glColor4fv(mColor);
		float y = vg::getTextAscender(mFont)+vg::getTextDescender(mFont);
		vg::drawString(mFont, 0, y/*(mHeight+y)/2*/, mText.c_str());
	}

	void Label::onAllocate()
	{
		mHeight = vg::getTextVExtent(mFont);
		mWidth  = vg::getTextHExtent(mFont, mText.c_str());
	}

	void Edit::onKeyDown(const KeyEvent& event/*u32 key*/)
	{
		u32 key = event.keysym.sym;
		// filter for special keys
		// Enter, quit edition
		if (key == SDLK_RETURN/*GLFW_KEY_ENTER*/)
		{
			return;
		}
		// Regular char, append it to the edited string
		else if (event.keysym.unicode>=32)
		{
			std::wstring::iterator	where = mText.end();
			if (mCaretPos < (int)mText.length())
			{
				where = mText.begin()+mCaretPos;
			}
			mText.insert(where, event.keysym.unicode);
			mCaretPos++;
			resetCursor();
		}
		// Special keys
		else
		{
			switch (key)
			{
			case SDLK_LEFT:
				{
					// move cursor left one char
					mCaretPos--;
				} 
				break;
			case SDLK_RIGHT:
				{
					// move cursor right one char
					mCaretPos++;
				} 
				break;
			case SDLK_HOME:
				{
					mCaretPos = 0;
				} 
				break;
			case SDLK_END:
				{
					mCaretPos = (int)mText.length();
				} 
				break;
			case SDLK_INSERT:
				{
				} 
				break;
			case SDLK_DELETE:
				{
					if (mCaretPos < (int)mText.length())
						mText.erase(mText.begin()+mCaretPos);
				} 
				break;
			case SDLK_BACKSPACE:
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
			resetCursor();
		}	
		mCaretPos = std::min(mCaretPos, (int)mText.length());
		mCaretPos = std::max(mCaretPos, 0);
	}

	void Edit::onPaint()
	{
		sui::drawRoundedRectOutline(0, 0, mWidth, mHeight, 5, 5, 3);
				
		glColor4fv(mColor);
		float y = (mHeight+vg::getTextAscender(mFont)+vg::getTextDescender(mFont))/2;
		vg::drawString(mFont, 7, y, mText.c_str());

		if (mShowCursor)
		{
			float tw = 7+vg::getTextHExtent(mFont, mText.substr(0, mCaretPos).c_str());
			sui::drawRect(tw, y-vg::getTextDescender(mFont), tw+1, y-vg::getTextAscender(mFont), 4, 4);
		}
	}
			
	void Edit::showCursor()
	{
		mShowCursor = true;
		mBlinkTask = mt::addTimedTask<Edit, &Edit::onBlink>(this, 400);
	}

	void Edit::hideCursor()
	{
		mShowCursor = false;
		if (mBlinkTask)
		{
			mt::terminateTask(mBlinkTask);
			mBlinkTask = 0;
		}
	}

	void Edit::resetCursor()
	{
		mShowCursor = true;
	}

	void Button::onPaint()
	{
		sui::drawFrame(0, 0, mWidth, mHeight, 5, 5, mIsHover, mIsPressed, false);
		glColor4fv(mColor);
		float x = vg::getTextHExtent(mFont, mText.c_str());
		float y = vg::getTextAscender(mFont)+vg::getTextDescender(mFont);
		vg::drawString(mFont, (mWidth-x)/2, (mHeight+y)/2, mText.c_str());
	}

	void CheckBox::onTouch(const ButtonEvent& event)
	{
		mIsChecked = !mIsChecked;
		Button::onTouch(event);
	}

	void CheckBox::onPaint()
	{
		sui::drawBoolFrame(0, 0, 16, 16, 16/6, 16/6, mIsHover, mIsChecked, false );
		glColor4fv(mColor);
		float x = 20;
		float y = vg::getTextAscender(mFont)+vg::getTextDescender(mFont);
		y = (16+y)/2;
		vg::drawString(mFont, x, y, mText.c_str());
	}

	void VBox::onAllocate()
	{
		//update childs extents;
		Container::onAllocate();

		float x = 0, y = mVMargin, w = getWidth();
		ChildsVector::iterator	it  = mChilds.begin(),
								end = mChilds.end();

		for (; it != end; ++it)
		{
			Actor&	actor = *(*it);
					
			switch (mAlign)
			{
				case Left:
					x = mHMargin;
					break;
				case Center:
					x = (w-actor.getWidth())/2.0f-mHMargin;
					break;
				case Right:
					x = w-mHMargin-actor.getWidth();
					break;
			}

			actor.setPos(x, y);
			y += actor.getHeight()+mSpacing;
		}
	}

	void VBox::onPaint()
	{
		float w = getWidth(), h = getHeight(), x = 0, y = 0;
		sui::drawFrame(x, y, w, h, 5, 5, false, false, false);
	}

	ProfileStatsBox::ProfileStatsBox()
	{
		setSpacing(4);
		setHMargin(15);
		setVMargin(7);
		setAlign(ui::VBox::Left);
		add(mVCNameLabel);
		add(mCPUTimeLabel);
		add(mGPUTimeLabel);
				
		wchar_t str[256];
		mbstowcs(str, (char*)glGetString(GL_RENDERER), 256);
		mVCNameLabel.setText(str);

		setStats(0, 0);
	}

	ProfileStatsBox& ProfileStatsBox::setFont(vg::Font font)
	{
		mVCNameLabel.setFont(font);
		mCPUTimeLabel.setFont(font);
		mGPUTimeLabel.setFont(font);

		return *this;
	}

	ProfileStatsBox& ProfileStatsBox::setStats(float cpuTime, float gpuTime)
	{
		wchar_t str[256];
		_snwprintf(str, 256, L"CPU time - %f ms", cpuTime);
		mCPUTimeLabel.setText(str);
		_snwprintf(str, 256, L"GPU time - %f ms", gpuTime);
		mGPUTimeLabel.setText(str);

		return * this;
	}
}