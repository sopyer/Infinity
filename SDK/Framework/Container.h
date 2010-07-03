#ifndef __CONTAINER_H_INCLUDED__
#	define __CONTAINER_H_INCLUDED__

#include <algorithm>

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
			virtual void onAllocate()
			{
				ChildsVector::iterator	it = mChilds.begin(),
										end = mChilds.end();
				for (; it != end; ++it)
				{
					(*it)->onAllocate();
				}
			}
			
		protected:
			typedef std::vector<Actor*> ChildsVector;
			
			ChildsVector	mChilds;
	};
}

#endif