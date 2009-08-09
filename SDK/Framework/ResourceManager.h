#pragma once

#include <map>
#include <string>
#include <Singleton.h>
#include <boost\smart_ptr.hpp>

template <class ManagerType, class ResourceType>
class ResourceManager : public Singleton<ManagerType>
{
private:
	typedef boost::shared_ptr<ResourceType> ReferenceBase;
	typedef boost::weak_ptr<ResourceType>	WeakReference;

public:
	class Reference : public ReferenceBase
	{
		friend class ResourceManager;

		public:
			Reference() {}
			Reference(ResourceType* ptr) : ReferenceBase(ptr) {}
			Reference(const Reference& ref) : ReferenceBase(ref) {}
			Reference(WeakReference weakRef) : ReferenceBase(weakRef) {}

			bool create(const std::string& name) {swap(ManagerType::getRef().get(name)); mName  = name; return get() != 0;}
			
			template<typename T1>
			bool create(const std::string& name, const T1& p1) {swap(ManagerType::getRef().get(name, p1)); mName  = name; return get() != 0;}

			template<typename T1, typename T2>
			bool create(const std::string& name, const T1& p1, const T2& p2) {swap(ManagerType::getRef().get(name, p1, p2)); mName  = name; return get() != 0;}

		private:
			std::string mName;
	};
	
	public:
		ResourceType* load() {return new ResourceType();}

		Reference get(const std::string& name)
		{
			Reference	ref;
			ResIt		it = mResources.find(name);
			if (it == mResources.end())
			{
				ref = Reference(getRef().load());
				mResources[name] = ref;
			}

			return ref;
		}

		template<typename T1>
		Reference get(const std::string& name, const T1& p1)
		{
			Reference	ref;
			ResIt		it = mResources.find(name);
			if (it == mResources.end())
			{
				ref = Reference(getRef().load(p1));
				mResources[name] = ref;
			}
			else
			{
				ref = Reference((*it).second);
			}

			return ref;
		}

		template<typename T1, typename T2>
		Reference get(const std::string& name, const T1& p1, const T2& p2)
		{
			Reference	ref;
			ResIt		it = mResources.find(name);
			if (it == mResources.end())
			{
				ref = Reference(getRef().load(p1, p2));
				mResources[name] = ref;
			}

			return ref;
		}

private:
	typedef std::map<std::string, /*WeakReference*/Reference>	ResourceMap;
	typedef typename ResourceMap::iterator	ResIt;

private:
		ResourceMap	mResources;
};