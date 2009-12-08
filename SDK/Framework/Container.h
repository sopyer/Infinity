#ifndef __CONTAINER_H_INCLUDED__
#	define __CONTAINER_H_INCLUDED__

namespace UI
{
	class Container: public Actor
	{
		friend class Stage;

		public:
			Container& add(Actor& child) {return add(&child);}
			Container& add(Actor* child)
			{
				if (!child)
					return *this;

				mChilds.push_back(child);
				child->setParent(this);
				queueRelayout();

				return *this;
			}

			Container& remove(Actor& child) {return remove(&child);}
			Container& remove(Actor* child) {std::remove(mChilds.begin(), mChilds.end(), child); child->setParent(0); return *this;}

		protected:
			void onPaint(VG& vg)
			{
				UNUSED(vg);
				//ChildsVector::iterator	it = mChilds.begin(),
				//						end = mChilds.end();
				//for (; it != end; ++it)
				//{
				//	if ((*it)->isVisible())
				//	{
				//		//Hack to apply transform for children
				//		glPushMatrix();
				//		//glTranslatef(mX, mY, 0);
				//		glMultMatrixf(getTransformMatrix()/*glm::translateGTX(mX, mY, 0.0f)*/);
				//		//glScalef(3, 3, 1);
				//		//glBegin(GL_QUADS);
				//		//glVertex2f(0, 0);
				//		//glVertex2f(0, 10);
				//		//glVertex2f(10, 10);
				//		//glVertex2f(10, 0);
				//		//glEnd();
				//		(*it)->onPaint(vg);
				//		glPopMatrix();
				//	}
				//}
			}

			virtual void onAllocate()
			{
				ChildsVector::iterator	it = mChilds.begin(),
										end = mChilds.end();
				for (; it != end; ++it)
				{
					(*it)->onAllocate();
				}
			}
			
			//Actor* pick(float x, float y)
			//{
			//	float relX = x - mX, relY = y - mY;
			//	ChildsVector::iterator	it = mChilds.begin(),
			//							end = mChilds.end();
			//	for (; it != end; ++it)
			//	{
			//		Actor* child = *it;
			//		Container* childContainer = dynamic_cast<Container*>(child);
			//		if (childContainer)
			//		{
			//			Actor* actor = childContainer->pick(relX, relY);
			//			if (actor) return actor;
			//		}
			//		
			//		if (child->pointInside(relX, relY))
			//		{
			//			return *it;
			//		}
			//	}

			//	return 0;
			//}
		protected:
			typedef std::vector<Actor*> ChildsVector;
			
			ChildsVector	mChilds;
	};
}

#endif