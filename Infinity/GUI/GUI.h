#pragma once

#include <gl\glew.h>
#include <FTFont.h>
#include <glm\glm.h>
#include <Framework.h>
#include <vector>
#include <algorithm>
#include <cassert>
#include <sigslot.h>

namespace gm
{
	class rect
	{
		friend glm::ivec2& leftBottom(rect&);
		friend glm::ivec2& rightTop(rect&);
		friend const glm::ivec2& leftBottom(const rect&);
		friend const glm::ivec2& rightTop(const rect&);

	public:
		rect();
		rect(const glm::ivec2& leftBottom, const glm::ivec2& rightTop);

	private:
		glm::ivec2	_leftBottom, _rightTop;
	};

	rect::rect()
	{}

	rect::rect(const glm::ivec2 &leftBottom, const glm::ivec2 &rightTop):
		_leftBottom(leftBottom), _rightTop(rightTop)
	{}

	glm::ivec2& leftBottom(rect& rc)
	{
		return rc._leftBottom;
	}

	glm::ivec2& rightTop(rect& rc)
	{
		return rc._rightTop;
	}

	const glm::ivec2& leftBottom(const rect& rc)
	{
		return rc._leftBottom;
	}

	const glm::ivec2& rightTop(const rect& rc)
	{
		return rc._rightTop;
	}

	void normalize(const rect& rc)
	{
		rect temp(glm::min(leftBottom(rc), rightTop(rc)), glm::max(leftBottom(rc), rightTop(rc)));
	}

	glm::ivec2 getPos(const rect& rc)
	{
		return leftBottom(rc);
	}

	glm::ivec2 getExtents(const rect& rc)
	{
		return rightTop(rc) - leftBottom(rc);
	}

	bool pointInside(const glm::ivec2& pt, const rect& rc)
	{
		return glm::all(glm::lessThanEqual(leftBottom(rc), pt)) && glm::all(glm::lessThanEqual(pt, rightTop(rc)));
	}

	rect intersect(const rect& rc1, const rect& rc2)
	{
		rect rc(glm::max(leftBottom(rc1), leftBottom(rc2)), glm::min(rightTop(rc1), rightTop(rc2)));
		return rc;
	}

	glm::ivec2 getCenter(const rect& rc)
	{
		return getPos(rc)+getExtents(rc)/2;
	}

	rect offset(const rect& rc, const glm::ivec2& val)
	{
		rect r(leftBottom(rc)+val, rightTop(rc) + val);
		return r;
	}
}

enum Justifying
{
	Left,
	Right,
	Center,
	Top,
	Bottom
};


//[TODO]: Change Painter/Scissor to equivalent functions(canvas api:))
class Scissor//: public Singleton<Scissor>
{
public:
	void enter(const glm::ivec2& pos, const glm::ivec2& size)
	{
		mStack.push_back(StackValue(pos, size));
		glScissor(pos.x, pos.y, size.x, size.y);
	}

	void subEnter(const glm::ivec2& pos, const glm::ivec2& size)
	{
		if (!mStack.empty())
		{
			glm::ivec2	newPos, newSize;

			newPos  = mStack.back().first + pos;
			newSize = glm::clamp(size, glm::ivec2(), mStack.back().second - pos);
			enter(newPos, newSize);
		}
		else
		{
			enter(pos, size);
		}
	}

	void leave()
	{
		mStack.pop_back();
		if (!mStack.empty())
			glScissor(mStack.back().first.x, mStack.back().first.y, mStack.back().second.x, mStack.back().second.y);
	}

	void use(bool yes)
	{
		if (yes)
			glEnable(GL_SCISSOR_TEST);
		else
			glDisable(GL_SCISSOR_TEST);
	}

private:
	typedef std::pair<glm::ivec2, glm::ivec2> StackValue;

	std::vector<StackValue>	mStack;
};

//[TODO]: Add Support of CSS
class Painter: public Scissor
{
	friend class Window;

public:
	Painter(const glm::ivec2& adjust):_adjust(adjust){}
	
	void setFont(FTFont* font)
	{
		_curFont = font;
	}

	void drawBox(const gm::rect& _rc)
	{
		glBegin(GL_QUADS);
			glVertex2iv(gm::leftBottom(_rc));
			glVertex2i(gm::leftBottom(_rc).x, gm::rightTop(_rc).y);
			glVertex2iv(gm::rightTop(_rc));
			glVertex2i(gm::rightTop(_rc).x, gm::leftBottom(_rc).y);
		glEnd();
	}

	void drawBox(const glm::ivec2& leftBottom,
				 const glm::ivec2& rightTop,
				 const glm::vec3& borderColor,
				 const glm::vec3& gradientFrom,
				 const glm::vec3& gradientTo)
	{
		glBegin(GL_QUADS);
			glColor3fv(gradientTo);
			glVertex2iv(leftBottom);
			glColor3fv(gradientFrom);
			glVertex2i(leftBottom.x, rightTop.y);
			glColor3fv(gradientFrom);
			glVertex2iv(rightTop);
			glColor3fv(gradientTo);
			glVertex2i(rightTop.x, leftBottom.y);
		glEnd();

		glColor3fv(borderColor);
		glBegin(GL_LINE_STRIP);
			glVertex2iv(leftBottom);
			glVertex2i(leftBottom.x, rightTop.y);
			glVertex2iv(rightTop);
			glVertex2i(rightTop.x, leftBottom.y);
			glVertex2iv(leftBottom-glm::ivec2(1,0));
		glEnd();
	}

	void drawText(const glm::ivec2& pos, const char* text, 
		Justifying hor = Left, Justifying vert = Bottom)
	{
		float x1, y1, z1, x2, y2, z2;
		_curFont->BBox(text, x1, y1, z1, x2, y2, z2);
		float h = y2-y1, w = x2-x1, dx = 0, dy = 0;
		
		glm::ivec2 _pos = pos;

		if (hor == Right)
			dx = -w;
		else if (hor == Center)
			dx = -w/2;
		
		if (vert == Top)
			dy = -h;
		else if (vert == Center)
			dy = -h/2;

		glPushMatrix();
			glTranslatef(_pos.x+dx, _pos.y+dy-y1, 0);
			//glColor3f(1, 0, 0);
			//glBegin(GL_LINES);
			//glVertex2f(100.0, 0);
			//glVertex2f(-100.0, 0);
			//glVertex2f(0, 100.0);
			//glVertex2f(0, -100.0);
			//glEnd();
			//glBegin(GL_LINE_LOOP);
			//glVertex2f(x1, y1);
			//glVertex2f(x1, y2);
			//glVertex2f(x2, y2);
			//glVertex2f(x2, y1);
			//glEnd();
			_curFont->Render(text);
		glPopMatrix();
	}


private:
	FTFont* _curFont;
	std::vector<gm::rect> _rects;
	glm::ivec2	_adjust;
};

class Text
{
public:
	Text(const char* text, FTFont* font, 
		Justifying hor, Justifying vert, 
		const glm::ivec2& pos):
						_text(text), _font(font), _pos(pos)
	{
		float x1, y1, z1, x2, y2, z2;
		_font->BBox(text, x1, y1, z1, x2, y2, z2);
		float h = y2-y1, w = x2-x1;
		
		if (hor == Right)
			_pos.x -= (int)w;
		else if (hor == Center)
			_pos.x -= (int)w/2;
		
		if (vert == Top)
			_pos.y -= (int)h;
		else if (vert == Center)
			_pos.y -= (int)h/2;
	}

	void draw()
	{
		glPushMatrix();
			glTranslatef((float)_pos.x, (float)_pos.y, 0);
			_font->Render(_text.c_str());
		glPopMatrix();
	}

private:
	std::string	_text;
	FTFont*		_font;
	glm::ivec2	_pos;
};

class Box
{
public:
	Box(const glm::vec3& color, const gm::rect& rc):
						_color(color), _rc(rc)
	{
	}

	void draw()
	{
		glColor3fv(_color);
		glBegin(GL_QUADS);
			glVertex2iv(gm::leftBottom(_rc));
			glVertex2i(gm::leftBottom(_rc).x, gm::rightTop(_rc).y);
			glVertex2iv(gm::rightTop(_rc));
			glVertex2i(gm::rightTop(_rc).x, gm::leftBottom(_rc).y);
			glVertex2iv(gm::leftBottom(_rc));
		glEnd();
	}

private:
	glm::vec3	_color;
	gm::rect	_rc;
};


enum MouseAction{maEnter, maLeave, maMove, maButton};

struct MouseEvent
{
	glm::ivec2	mPos;
	int			mButton;
	int			mState;
	MouseAction	mAction;

	MouseEvent(const glm::ivec2& pos, int button, int state, MouseAction action):
		mPos(pos), mState(state), mButton(button), mAction(action)
	{}
};

//[TODO]: Add color scheme support like css
class Element
{
	friend class Container;
public:
	Element(): mVisible(true), mEnable(true) {}

	Element&	show() {mVisible = true; return *this;}
	Element&	hide() {mVisible = false; return *this;}

	Element&	enable()  {mEnable = true; return *this;}
	Element&	disable() {mEnable = false; return *this;}

	Element&	setPos(const glm::ivec2& pos)   {mPos = pos; return *this;}
	Element&	setSize(const glm::ivec2& size) {mSize = size; return *this;}

	bool		isEnabled() {return mEnable;}
	bool		isVisible() {return mVisible;}

	glm::ivec2	getPos()  {return mPos;}
	glm::ivec2	getSize() {return mSize;}

protected:
	virtual void onDraw(Painter& painter){}
	virtual void onMouseEvent(const MouseEvent& event){}
	virtual void onFocus(){}
	virtual void onKey(){}
	virtual void onMouseWheel(){}

protected:
	bool	pointInside(const glm::ivec2& pt)
	{
		return	glm::all(glm::lessThanEqual(mPos, pt)) && 
				glm::all(glm::lessThan(pt, mPos+mSize));
	}

protected:
	glm::ivec2	mPos, mSize;
	bool		mVisible:1, mEnable:1;
};

//[TODO]: add tiled layout support, like android API
class Container: public Element
{
public:
	Container(): mLastOver(0){}

	void add(Element& child) {mChilds.push_back(&child);}
	void add(Element* child) {mChilds.push_back(child);}

protected:
	void onDraw(Painter& painter)
	{
		glm::ivec2	pos = getPos();

		glPushMatrix();
		glTranslatef(pos.x, pos.y, 0);

		ChildsVector::iterator	it = mChilds.begin(),
								end = mChilds.end();

		for (; it != end; ++it)
		{
			if ((*it)->isVisible())
			{
				painter.subEnter((*it)->getPos()-glm::ivec2(1, 1), (*it)->getSize()+glm::ivec2(1, 1));
				(*it)->onDraw(painter);
				painter.leave();
			}
		}

		glPopMatrix();
	}

	void onMouseEvent(const MouseEvent& event)/*what about modifiers?????*/
	{
		Element*	target = 0;
		MouseEvent	newEvent(event);

		ChildsVector::reverse_iterator	it  = mChilds.rbegin(),
										end = mChilds.rend();

		for (; it != end; ++it)
		{
			if ((*it)->isVisible() && (*it)->pointInside(event.mPos))
			{
				target = *it;
				break;
			}
		}

		if (mLastOver)
		{
			if (target != mLastOver)
			{
				newEvent.mPos = event.mPos - mLastOver->getPos();
				newEvent.mAction = maLeave;
				mLastOver->onMouseEvent(newEvent);
			}
		}

		if (target)
		{
			if (target != mLastOver && event.mAction == maMove)
				newEvent.mAction = maEnter;

			newEvent.mPos = event.mPos - target->getPos();
			(*it)->onMouseEvent(newEvent);
			mLastOver = *it;
		}
	}

	void onFocus(){}
	void onKey(){}
	void onMouseWheel(){}

private:
	typedef std::vector<Element*> ChildsVector;

	Element*		mLastOver;
	ChildsVector	mChilds;
};

class Frame: public Container
{
public:
	Frame(const gm::rect& rc, const char* text, FTFont* font): 
		  mText(text), mFont(font), mTrackMouse(false)
	{
		this->setPos(gm::leftBottom(rc)).
			  setSize(gm::getExtents(rc));
	}

protected:
	void onMouseEvent(const MouseEvent& event)
	{
		if (event.mAction == maButton && event.mState & GLFW_PRESS)
		{
			mTrackMouse = true;
			mBaseCoord  = event.mPos;
		}

		if (event.mAction == maLeave || (event.mState & GLFW_PRESS) == 0)
		{
			mTrackMouse = false;
		}
		
		if (mTrackMouse)
		{
			setPos(getPos()+event.mPos-mBaseCoord);
		}

		Container::onMouseEvent(event);
	}

	void onDraw(Painter& painter)
	{
		painter.drawBox(getPos(), 
						getPos()+getSize() - glm::ivec2(1, 1), 
						glm::vec3(0.18, 0.07, 0.07),
						glm::vec3(0.05, 0.05, 0.05),
						glm::vec3(0.05, 0.05, 0.05));

		glBegin(GL_LINE_STRIP);
			glVertex2i(getPos().x,				getPos().y+getSize().y-30);
			glVertex2i(getPos().x+getSize().x,	getPos().y+getSize().y-30);
		glEnd();

		glColor3f(1.0f, 1.0f, 1.0f);
		painter.setFont(mFont);
		painter.drawText(getPos()+glm::ivec2(getSize().x/2, getSize().y - 15), mText.c_str(), Center, Center);

		Container::onDraw(painter);
	}

private:
	bool		mTrackMouse;
	glm::ivec2	mBaseCoord;
	std::string	mText;
	FTFont*		mFont;
};

//[TODO]: 
//	1)Fix Enter/Leave issue on entire window region
//	2)MouseButton represents pressed buttons; MouseEvent+=(maButtonUp, maButtonDown)
//	3)Add modifiers(ctrl, alt, shift) state
//  Remake using glfw and sfml code
class Window: public Container, public Framework/*in future should only rely on low level library like glfw or platform API*/
{
public:
	Window():mButton(0), mState(0)
	{
		setPos(glm::ivec2(0,0)).
		setSize(glm::ivec2(width_, height_));

		glfwSetMousePosCallback(&mouseMoveCallback);
		glfwSetMouseButtonCallback(&mouseButtonCallback);
	}

protected:
	void OnRender()
	{
		Painter pai(glm::ivec2(0,0));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_SCISSOR_TEST);

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushMatrix();
		glTranslatef(-width_/2.0f, -height_/2.0f, 0);

		pai.enter(glm::ivec2(), glm::ivec2(width_, height_));
		onDraw(pai);
		pai.leave();

		glPopMatrix();
		glPopAttrib();

		glDisable(GL_SCISSOR_TEST);
		glDisable(GL_TEXTURE_2D);
	}

private:
	int			mButton;
	int			mState;
	glm::ivec2	mMousePos;

	static void emitMouseEvent(MouseAction action)
	{
		Window* w = dynamic_cast<Window*>(Framework::getPtr());
		w->onMouseEvent(MouseEvent(glm::ivec2(w->mMousePos.x, w->height_ - w->mMousePos.y),
								   w->mButton,
								   w->mState,
								   action
								  )
					   );
	}

	static void mouseMoveCallback(int x, int y)
	{
		Window* w = dynamic_cast<Window*>(Framework::getPtr());
		w->mMousePos = glm::ivec2(x, y);
		emitMouseEvent(maMove);
	}

	static void mouseButtonCallback(int button, int state)
	{
		Window* w = dynamic_cast<Window*>(Framework::getPtr());
		w->mButton = button;
		w->mState  = state;
		emitMouseEvent(maButton);
	}
};

class Button: public Element
{
public:
	Button(const gm::rect& rc, const char* text, FTFont* font): 
						Element(), mState(bsNormal), mText(text), mFont(font)
	{
		this->setPos(gm::leftBottom(rc)).
			setSize(gm::getExtents(rc));
		mGradientColorFrom[bsNormal] = glm::vec3(0.08, 0.08, 0.08);
		mGradientColorFrom[bsFocused] = glm::vec3(0.13, 0.13, 0.13);
		mGradientColorFrom[bsClicked] = glm::vec3(0.13, 0.05, 0.05);
		mGradColorTo[bsNormal] = glm::vec3(0.01, 0.01, 0.01);
		mGradColorTo[bsFocused] = glm::vec3(0.05, 0.05, 0.05);
		mGradColorTo[bsClicked] = glm::vec3(0.07, 0.05, 0.05);
	}

public:
	sigslot::signal0<>	clicked;

protected:
	void onDraw(Painter& painter)
	{
		assert(0 <= mState && mState < bsStateCount);
		painter.drawBox(getPos(), 
						getPos()+getSize() - glm::ivec2(1, 1), 
						glm::vec3(0.18, 0.07, 0.07),
						mGradientColorFrom[mState],
						mGradColorTo[mState]);
		glColor3f(0.97f, 0.12f, 0.12f);
		painter.setFont(mFont);
		painter.drawText(gm::getCenter(gm::rect(getPos(), getPos()+getSize())), mText.c_str(), Center, Center);
	}

	void onMouseEvent(const MouseEvent& event)
	{

		mMouseOver = true;
		if (event.mAction == maLeave)
		{
			mMouseOver = false;
			mState     = bsNormal;
		}
		else
		{
			mState = bsFocused;

			if (event.mState & GLFW_PRESS)
				mState = bsClicked;

			if (event.mAction == maButton &&
				(event.mState & GLFW_PRESS) == 0)
			{
				mState = bsFocused;
				clicked();
			}
		}
	}

private:
	enum ButtonState{bsNormal, bsFocused, bsClicked, bsStateCount};

	bool		mMouseOver;
	std::string	mText;
	ButtonState	mState;
	FTFont*		mFont;
	glm::vec3	mGradientColorFrom[bsStateCount];
	glm::vec3	mGradColorTo[bsStateCount];
};

inline glm::ivec2 tokenize(const std::string str, const char* tokens, int& start )
{
	glm::ivec2	word(0);
	if (start >= 0 && start < (int)str.length())
	{	
		start = str.find_first_not_of(tokens, start);
		int end = str.find_first_of(tokens, start);
		
		if (start != std::string::npos)
		{
			if (end != std::string::npos)
			{
				word = glm::ivec2(start, end - start);
			}
			else
			{
				word = glm::ivec2(start, str.length() - start);
			}
		}

		start = end;
	}

	return word;
}

class Label: public Element
{
public:
	Label(const gm::rect& rc, const char* text, FTFont* font): 
						mText(text), mFont(font)
	{
		this->setPos(gm::leftBottom(rc)).
			setSize(gm::getExtents(rc));
		
		int start = 0;
		while (start != std::string::npos)
		{
			mWords.push_back(tokenize(mText, " ", start));
		}
	}

protected:
	void onDraw(Painter& painter)
	{
		glColor3f(1.0f, 1.0f, 1.0f);
		painter.setFont(mFont);
		painter.drawText(gm::getCenter(gm::rect(getPos(), getPos()+getSize())), mText.c_str(), Center, Center);
	}

private:
	std::vector<glm::ivec2>	mWords;
	std::string				mText;
	FTFont*					mFont;
};