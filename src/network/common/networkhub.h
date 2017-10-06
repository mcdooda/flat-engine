#ifndef FLAT_NETWORK_COMMON_NETWORKHUB_H
#define FLAT_NETWORK_COMMON_NETWORKHUB_H

#include <vector>
#include <memory>
#include "networkevent.h"
#include "../../containers/dynamicpool.h"
#include "../../util/instancer.h"

namespace flat
{
namespace network
{
namespace common
{

class NetworkHub
{
	private:
		struct EventPool
		{
			flat::containers::DynamicPool pool;
			std::vector<NetworkEvent*> allocatedObjects;
			flat::util::Instancer<NetworkEvent> instancer;
		};

	public:
		NetworkHub();

		template <class T>
		void registerEvent();

		template <class T>
		T& newEvent();

		NetworkEvent& newEventFromDelimiter(NetworkEvent::Delimiter delimiter);

		template <class T, typename... Args>
		T& initEvent(const Args... args);

	private:
		void clearEvents();

		EventPool& getEventPoolFromDelimiter(NetworkEvent::Delimiter delimiter) const;

	private:
		std::vector<std::unique_ptr<EventPool>> m_eventPools;
		NetworkEvent::Delimiter m_nextEventDelimiter;
};

template<class T>
inline void NetworkHub::registerEvent()
{
	T::setDelimiter(m_nextEventDelimiter);
	m_nextEventDelimiter++;

	EventPool* eventPool = new EventPool();
	eventPool->pool.init<T, 50>();
	m_eventPools.emplace_back(eventPool);
	eventPool->instancer.makeInstancer<T>();
}

template<class T>
inline T& NetworkHub::newEvent()
{
	EventPool& eventPool = getEventPoolFromDelimiter(T::getDelimiter());
	T* event = eventPool.pool.create<T>();
	eventPool.allocatedObjects.push_back(event);
	return *event;
}

template<class T, typename... Args>
inline T& NetworkHub::initEvent(const Args ...args)
{
	T& event = newEvent<T>();
	event.init(args...);
	return event;
}

} // common
} // network
} // flat

#endif // FLAT_NETWORK_COMMON_NETWORKHUB_H


