#ifndef FLAT_RESOURCE_RESOURCEMANAGER_H
#define FLAT_RESOURCE_RESOURCEMANAGER_H

#include <map>
#include <tuple>

namespace flat
{
namespace resource
{

template <class T, typename... U>
class ResourceManager
{
	protected:
		using CacheKeyType = std::tuple<U...>;
		using CacheValueType = std::weak_ptr<const T>;
		using CacheType = std::map<CacheKeyType, CacheValueType>;
		
	public:
		std::shared_ptr<const T> getResource(const U&... initializers) const
		{
			CacheKeyType initializersTuple(initializers...);
			typename CacheType::iterator it = m_loadedResources.find(initializersTuple);
			if (it != m_loadedResources.end())
				if (std::shared_ptr<const T> sharedResource = it->second.lock())
					return sharedResource;
	
			const T* resource = new T(initializers...);
			std::shared_ptr<const T> sharedResource;
			sharedResource.reset(resource);
			m_loadedResources[initializersTuple] = CacheValueType(sharedResource);
			return sharedResource;
		}

	protected:
		mutable CacheType m_loadedResources;
};

} // resource
} // flat

#endif // FLAT_RESOURCE_RESOURCEMANAGER_H


