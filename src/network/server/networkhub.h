#ifndef FLAT_NETWORK_SERVER_NETWORKHUB_H
#define FLAT_NETWORK_SERVER_NETWORKHUB_H

#include <unordered_map>

#include "clientnetworkeventhandler.h"

#include "../common/networkhub.h"

namespace flat
{
namespace network
{
namespace server
{
class Client;

class NetworkHub : public common::NetworkHub
{
public:
	NetworkHub();

	template <class EventType, class ObjectType>
	inline void registerEventHandler(ObjectType* object, void (ObjectType::*handler)(const Client& client, const EventType& event));

private:
	std::unordered_map<common::NetworkEvent::Delimiter, std::unique_ptr<ClientNetworkEventHandler>> m_handlers;
};

template<class EventType, class ObjectType>
inline void NetworkHub::registerEventHandler(ObjectType* object, void (ObjectType::* handler)(const Client& client, const EventType& event))
{
	NetworkEvent::Delimiter delimiter = EventType::getDelimiter();
	m_handlers[delimiter] = std::make_unique<ClientNetworkEventHandlerImpl<EventType, ObjectType>>(object, handler);
}

} // server
} // network
} // flat

#endif // FLAT_NETWORK_SERVER_NETWORKHUB_H


