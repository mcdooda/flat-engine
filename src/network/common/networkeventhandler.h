#ifndef FLAT_NETWORK_COMMON_NETWORKEVENTHANDLER_H
#define FLAT_NETWORK_COMMON_NETWORKEVENTHANDLER_H

namespace flat
{
namespace network
{
namespace common
{
class NetworkEvent;
class Peer;

class NetworkEventHandler
{
	public:
		virtual void handleEvent(const common::NetworkEvent& event, Peer& peer) const = 0;
};

template <class EventType, class ObjectType>
class NetworkEventHandlerImpl : public NetworkEventHandler
{
	public:
		using Callback = void (ObjectType::*)(const EventType&, Peer&);
	public:
		NetworkEventHandlerImpl(ObjectType* object, Callback callback);

		void handleEvent(const common::NetworkEvent& event, Peer& peer) const override;

	private:
		ObjectType* m_object;
		Callback m_handler;
};

template<class EventType, class ObjectType>
void NetworkEventHandlerImpl<EventType, ObjectType>::handleEvent(const common::NetworkEvent& event, Peer& peer) const
{
	const EventType& typedEvent = static_cast<const EventType&>(event);
	(m_object->*m_handler)(typedEvent, peer);
}

} // common
} // network
} // flat

#endif // FLAT_NETWORK_COMMON_NETWORKEVENTHANDLER_H


