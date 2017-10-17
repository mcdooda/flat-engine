#include "networkhub.h"
#include "..\client\networkhub.h"

namespace flat
{
namespace network
{
namespace common
{

NetworkHub::NetworkHub() :
	m_nextEventDelimiter(1)
{
}

NetworkEvent& NetworkHub::newEventFromDelimiter(NetworkEvent::Delimiter delimiter)
{
	EventPool& eventPool = getEventPoolFromDelimiter(delimiter);
	return *eventPool.instancer.makeObject(eventPool.pool);
}

void NetworkHub::send(const NetworkEvent& event, Peer& peer)
{
	
}

void NetworkHub::clearEvents()
{
	for (const std::unique_ptr<EventPool>& eventPoolPtr : m_eventPools)
	{
		EventPool* eventPool = eventPoolPtr.get();
		for (NetworkEvent* event : eventPool->allocatedObjects)
		{
			eventPool->pool.destroy(event);
		}
		eventPool->allocatedObjects.clear();
	}
}

NetworkHub::EventPool& NetworkHub::getEventPoolFromDelimiter(NetworkEvent::Delimiter delimiter) const
{
	FLAT_ASSERT(delimiter > 0);
	return *m_eventPools[delimiter - 1].get();
}

} // common
} // network
} // flat


