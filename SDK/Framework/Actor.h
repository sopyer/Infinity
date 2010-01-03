#ifndef __ACTOR_H_INCLUDED__
#	define __ACTOR_H_INCLUDED__

#	include <utils.h>
#	include <glm\glm.h>

class Actor;
class VG;
class Stage;
class Container;

struct ButtonEvent;
struct MotionEvent;
struct KeyEvent;

namespace UI
{
	struct Color
	{
		u8	red, green, blue, alpha;
	};

	//Determine relayout points, take Clutter as reference
	class Actor
	{
		friend class Container;
		friend class Stage;

		public:
			Actor(): mParent(0), mVisible(true), mEnabled(true),
				mX(0), mY(0), mZ(0),
				mWidth(0), mHeight(0), mScaleX(1), mScaleY(1),
				mAngle(0), mRotDirX(0), mRotDirY(0), mRotDirZ(0)
			{}

			Actor(Actor* parent): mParent(parent), mVisible(true), mEnabled(true),
				mX(0), mY(0), mZ(0),
				mWidth(0), mHeight(0), mScaleX(1), mScaleY(1),
				mAngle(0), mRotDirX(0), mRotDirY(0), mRotDirZ(0)
			{}

			Actor&	show() {mVisible = true; queueRelayout(); return *this;}
			Actor&	hide() {mVisible = false; queueRelayout(); return *this;}

			Actor&	enable()  {mEnabled = true; return *this;}
			Actor&	disable() {mEnabled = false; return *this;}
			
			Actor&	queueRelayout();

			Stage*	getStage();

			Actor&	setPos(float x, float y)   {mX = x; mY = y; queueRelayout(); return *this;}
			Actor&	setSize(float width, float height) {mWidth = width; mHeight = height; queueRelayout(); return *this;}
			
			Actor&	setParent(Actor& parent) {return setParent(&parent);}
			//!!!! Resolve add/remove problem while setting parent-child relation
			Actor&	setParent(Actor* parent);// {dynamic_cast<Container>(mParent)->remove(this); mParent = parent; return *this;}

			Actor&	setWidth(float w)		{mWidth = w; return *this;}
			Actor&	setHeight(float h)	{mHeight = h; return *this;}

			Actor&	setX(float x) {mX = x; return *this;}
			Actor&	setY(float y) {mY = y; return *this;}
			Actor&	setZ(float z) {mZ = -z; return *this;}

			Actor&	setScaleX(float scaleX) {mScaleX = scaleX; return *this;}
			Actor&	setScaleY(float scaleY) {mScaleY = scaleY; return *this;}
			
			Actor&	setRotation(float angle, float rotDirX, float rotDirY, float rotDirZ)
			{mAngle = angle; mRotDirX = rotDirX;  mRotDirY = rotDirY;  mRotDirZ = rotDirZ; return *this;}

			float	getWidth()	{return mWidth;}
			float	getHeight()	{return mHeight;}

			float	getX() {return mX;}
			float	getY() {return mY;}
			float	getZ() {return mZ;}
			
			Actor*	getParent() {return mParent;}

			bool		isEnabled() {return mEnabled;}
			bool		isVisible() {return mVisible;}

		protected:
			virtual void onTouch(const ButtonEvent& event) {UNUSED(event);}
			virtual void onUntouch(const ButtonEvent& event) {UNUSED(event);}

			virtual void onMotion(const MotionEvent& event) {UNUSED(event);}

			virtual void onScroll() {}

			virtual void onKeyDown(const KeyEvent& event) {UNUSED(event);}
			virtual void onKeyUp(const KeyEvent& event) {UNUSED(event);}
			
			virtual void onEnter() {}
			virtual void onLeave() {}

			virtual void onFocusIn() {}
			virtual void onFocusOut() {}

			virtual void onPaint() {}
			virtual void onPick(Color color);
			virtual void onAllocate() {}
		
		protected:
			bool	pointInside(float x, float y)
			{
				return	(mX <= x) && 
						(mY <= y) &&
						(x < mX+mWidth) && 
						(y < mY+mHeight);
			}

			glm::mat4 getTransformMatrix()
			{
				glm::mat4 mat;

				mat = glm::translateGTX(mX, mY, mZ);
				if (mRotDirX+mRotDirY+mRotDirZ != 0.0f)
					mat	*= glm::rotateGTX(mAngle, mRotDirX, mRotDirY, mRotDirZ);
				mat *= glm::scaleGTX(mScaleX, mScaleY, 1.0f);
				//mat *= glm::translateGTX(-mWidth/2, -mHeight/2, 0.0f);

				 return mat;
			}

		protected:
			Actor*		mParent;

			float		mX, mY, mZ;
			float		mScaleX, mScaleY;
			float		mAngle, mRotDirX, mRotDirY, mRotDirZ;

			float		mWidth, mHeight;
			bool		mVisible, mEnabled;
	};
}

#endif