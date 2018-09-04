#ifndef FLAT_RESOURCE_STRONGRESOURCEMANAGER_H
#define FLAT_RESOURCE_STRONGRESOURCEMANAGER_H

#include <memory>
#include "resourcemanager.h"

namespace flat
{
namespace resource
{

template <class T, typename... U>
class StrongResourceManager : public ResourceManager<T, std::shared_ptr, U...>
{
	public:
		std::shared_ptr<const T> getResource(const U&... initializers) const;
};

template<class T, typename... U>
inline std::shared_ptr<const T> StrongResourceManager<T, U...>::getResource(const U&... initializers) const
{
	CacheKeyType initializersTuple(initializers...);
	typename CacheType::iterator it = m_loadedResources.find(initializersTuple);
	if (it != m_loadedResources.end())
		if (std::shared_ptr<const T> sharedResource = it->second)
			return sharedResource;

	std::shared_ptr<const T> sharedResource = std::make_shared<const T>(initializers...);
	m_loadedResources[initializersTuple] = CacheValueType(sharedResource);
	return sharedResource;
}

} // resource
} // flat

#endif // FLAT_RESOURCE_STRONGRESOURCEMANAGER_H

