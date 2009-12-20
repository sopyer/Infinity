#ifndef __RESOURCEHANDLE_H_INCLUDED__
#define __RESOURCEHANDLE_H_INCLUDED__

#include <map>
#include <string>
#include <cassert>
#include <algorithm>

#if 1
#   include <iostream>
#endif

template<class T>
class ResourceIndex//: public Singleton< ResourceIndex<T> >
{
    public:
        ResourceIndex() {}

        void add(const char* name, T* res)
        {
            assert(res);
            assert(find(name)==0);
            mIndex[name] = res;
        }

        T* find(const char* name)
        {
            IndexIt it = mIndex.find(name);
            return it!=mIndex.end()?it->second:0;
        }

        void remove(T* res)
        {
            IndexIt it = std::find_if(mIndex.begin(), mIndex.end(), pred_search_ptr(res));
            if (it!=mIndex.end())
                mIndex.erase(it);
        }

    private:
        typedef std::map<std::string, T*>		IndexType;
        typedef typename IndexType::iterator	IndexIt;

        struct pred_search_ptr
        {
            pred_search_ptr(T* ptr): mPtr(ptr)
            {}

            bool operator ()(typename IndexType::value_type const & elem)
            {return elem.second==mPtr;}

            T*  mPtr;
        };

    private:
        IndexType   mIndex;
};

template<class T>
class ResourceHandle
{
	private:
		struct Holder
		{
			Holder(): mResource(), mRefCount()
			{}

			T       mResource;
			size_t  mRefCount;
		};

        typedef ResourceIndex< Holder >  Registry;

    public:
        ResourceHandle(): mObject(0)
        {}

        ResourceHandle(const char* name)
        {create(name);}

        ResourceHandle(const ResourceHandle& other): mObject(other.mObject)
        {addRef();}

        ~ResourceHandle()
        {release();}

        ResourceHandle& operator=(const ResourceHandle& other)
        {
            assign(other);
            return *this;
        }

        template<class U>
        operator U()
        {return mObject->mResource;}

        void reset()
        {ResourceHandle().swap(*this);}

        void swap(ResourceHandle& other)
        {
            Holder* tmp = mObject;
            mObject = other.mObject;
            other.mObject = tmp;
        }

#if 1   //UNIT_TESTS
        Holder* get()
        {return mObject;}
#endif

	protected:
		template<class Constructor>
        void create(const char* name, Constructor& ctor)
        {
            release();

            assign(mRegistry.find(name));
            if (!mObject)
            {
                assign(new Holder());
                ctor(mObject->mResource);
                mRegistry.add(name, mObject);
            }
        }

	private:
        template<class U>
        void assign(U other)
        {ResourceHandle(other).swap(*this);}

        ResourceHandle(Holder* other): mObject(other)
        {addRef();}

        void addRef()
        {if (mObject) mObject->mRefCount++;}

        void release()
        {
            if (mObject && --mObject->mRefCount==0)
            {
                mRegistry.remove(mObject);
                delete mObject;
            }
        }

    private:
        Holder* mObject;

        static Registry mRegistry;
};

template<class T>
typename ResourceHandle<T>::Registry ResourceHandle<T>::mRegistry;

template<class T> inline bool operator==(ResourceHandle<T> const & a, ResourceHandle<T> const & b)
{
    return a.get() == b.get();
}

template<class T> inline bool operator!=(ResourceHandle<T> const & a, ResourceHandle<T> const & b)
{
    return a.get() != b.get();
}

template<class T> inline bool operator<(ResourceHandle<T> const & a, ResourceHandle<T> const & b)
{
    return std::less<T *>()(a.get(), b.get());
}

template<class T> void swap(ResourceHandle<T> & lhs, ResourceHandle<T> & rhs)
{
    lhs.swap(rhs);
}

#endif // __RESOURCEHANDLE_H_INCLUDED__
