#ifndef FLAT_RESOURCE_RESOURCEMANAGER_H
#define FLAT_RESOURCE_RESOURCEMANAGER_H

#include <map>
#include <tuple>

namespace flat
{
namespace resource
{

template <class T, template<typename> class CacheValueContainer, typename... U>
class ResourceManager
{
	protected:
		using CacheKeyType = std::tuple<U...>;
		using CacheValueType = CacheValueContainer<const T>;
		using CacheType = std::map<CacheKeyType, CacheValueType>;

	protected:
		mutable CacheType m_loadedResources;
};

} // resource
} // flat

#endif // FLAT_RESOURCE_RESOURCEMANAGER_H


