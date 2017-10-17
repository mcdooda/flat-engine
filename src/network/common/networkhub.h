#ifndef FLAT_NETWORK_COMMON_NETWORKHUB_H
#define FLAT_NETWORK_COMMON_NETWORKHUB_H

#include <vector>
#include <memory>
#include <unordered_map>
#include "networkevent.h"
#include "networkeventhandler.h"
#include "../../containers/dynamicpool.h"
#include "../../util/instancer.h"

namespace flat
{
namespace network
{
namespace common
{
class Peer;

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

		template <class EventType, class ObjectType>
		inline void registerEventHandler(ObjectType* object, void (ObjectType::*handler)(const Peer& peer, const EventType& event));

	protected:
		void send(const NetworkEvent& event, Peer& peer);

	private:
		void clearEvents();

		EventPool& getEventPoolFromDelimiter(NetworkEvent::Delimiter delimiter) const;

	private:
		std::unordered_map<common::NetworkEvent::Delimiter, std::unique_ptr<NetworkEventHandler>> m_handlers;
		std::vector<std::unique_ptr<EventPool>> m_eventPools;
		std::vector<const NetworkEvent*> m_eventsToSend;
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

template<class EventType, class ObjectType>
inline void NetworkHub::registerEventHandler(ObjectType* object, void (ObjectType::* handler)(const Peer& peer, const EventType& event))
{
	NetworkEvent::Delimiter delimiter = EventType::getDelimiter();
	m_handlers[delimiter] = std::make_unique<NetworkEventHandlerImpl<EventType, ObjectType>>(object, handler);
}

} // common
} // network
} // flat

#endif // FLAT_NETWORK_COMMON_NETWORKHUB_H


