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
	typedef std::map<std::string, WeakReference>	ResourceMap;
	typedef typename ResourceMap::iterator	ResIt;

public:
	class Reference : public ReferenceBase
	{
		friend class ResourceManager;

		public:
			Reference() {}
			Reference(ResourceType* ptr) : ReferenceBase(ptr) {}

			bool create(const std::string& name) {swap(ManagerType::getRef().get(name)); mName  = name; return get() != 0;}
		
		private:
			std::string mName;
	};
	
	public:
		Reference get(const std::string& name)
		{
			Reference	ref;
			ResIt		it = mResources.find(name);
			if (it == mResources.end())
			{
				ref = Reference(getRef().load(name));
				mResources[name] = ref;
			}

			return ref;
		}
		
	private:
		ResourceMap	mResources;
};