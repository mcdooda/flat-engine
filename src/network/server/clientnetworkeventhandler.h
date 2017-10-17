#ifndef FLAT_NETWORK_SERVER_CLIENTNETWORKEVENTHANDLER_H
#define FLAT_NETWORK_SERVER_CLIENTNETWORKEVENTHANDLER_H

namespace flat
{
namespace network
{
namespace common
{
class NetworkEvent;
}
namespace server
{
class Client;

class ClientNetworkEventHandler
{
	public:
		virtual void handleEvent(const common::NetworkEvent& event, Client& client) const = 0;
};

template <class EventType, class ObjectType>
class ClientNetworkEventHandlerImpl : public ClientNetworkEventHandler
{
	public:
		using Callback = void (ObjectType::*)(const EventType&, Client&);
	public:
		ClientNetworkEventHandlerImpl(ObjectType* object, Callback callback);

		void handleEvent(const common::NetworkEvent& event, Client& client) const override;

	private:
		ObjectType* m_object;
		Callback m_handler;
};

template<class EventType, class ObjectType>
void ClientNetworkEventHandlerImpl<EventType, ObjectType>::handleEvent(const common::NetworkEvent& event, Client& client) const
{
	const EventType& typedEvent = static_cast<const EventType&>(event);
	(m_object->*m_handler)(typedEvent, client);
}

} // server
} // network
} // flat

#endif // FLAT_NETWORK_SERVER_CLIENTNETWORKEVENTHANDLER_H


