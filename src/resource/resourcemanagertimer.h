#ifndef FLAT_RESOURCE_RESOURCEMANAGERTIMER_H
#define FLAT_RESOURCE_RESOURCEMANAGERTIMER_H

#include <unordered_map>

#include "resourcemanager.h"

#include "../time/time.h"

namespace flat
{
namespace resource
{

template <class T, typename... U>
class ResourceManagerTimer : protected ResourceManager<T, U...>
{
	using Super = ResourceManager<T, U...>;
	protected:
		using KeepLoadedMapType = std::unordered_map<std::shared_ptr<const T>, float>;

	public:
		ResourceManagerTimer() = default;
		ResourceManagerTimer(const ResourceManagerTimer&) = delete;
		ResourceManagerTimer(ResourceManagerTimer&&) = default;

		ResourceManagerTimer(Flat& flat)
		{
			init(flat);
		}

		void init(Flat& flat)
		{
			m_clock = flat.time->newClock();
			m_update = 0;
		}

		std::shared_ptr<const T> getResource(float duration, const U&... initializers) const
		{
			FLAT_ASSERT_MSG(m_clock != nullptr, "Resource manager with timer not initialized");
			std::shared_ptr<const T> resource = Super::getResource(initializers...);
			const float expiryTime = m_clock->getTime() + duration;
			KeepLoadedMapType::iterator it = m_keepLoadedMap.find(resource);
			if (it != m_keepLoadedMap.end())
			{
				it->second = std::max(it->second, expiryTime);
			}
			else
			{
				m_keepLoadedMap[resource] = expiryTime;
			}
			return resource;
		}

		void update()
		{
			m_update = (m_update + 1) % UPDATE_FREQUENCY;
			if (m_update != 0)
			{
				return;
			}

			FLAT_ASSERT_MSG(m_clock != nullptr, "Resource manager with timer not initialized");
			const float currentTime = m_clock->getTime();
			for (KeepLoadedMapType::iterator it = m_keepLoadedMap.begin(); it != m_keepLoadedMap.end(); )
			{
				if (it->second < currentTime)
				{
					it = m_keepLoadedMap.erase(it);
				}
				else
				{
					++it;
				}
			}
		}

	protected:
		std::shared_ptr<time::Clock> m_clock;
		mutable KeepLoadedMapType m_keepLoadedMap;

		static constexpr int UPDATE_FREQUENCY = 10;
		mutable int m_update;
};

} // resource
} // flat

#endif // FLAT_RESOURCE_RESOURCEMANAGERTIMER_H


