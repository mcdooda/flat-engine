#ifndef FLAT_NETWORK_SERVER_NETWORKHUB_H
#define FLAT_NETWORK_SERVER_NETWORKHUB_H

#include <unordered_map>
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

	template <class T>
	inline void registerEventHandler(void (*handler)(const Client& client, const T& event));

private:
	std::unordered_map<common::NetworkEvent::Delimiter, void*> m_handlers;
};

template<class T>
inline void NetworkHub::registerEventHandler(void (*handler)(const Client& client, const T& event))
{
	common::NetworkEvent::Delimiter delimiter = T::getDelimiter();
	m_handlers[delimiter] = handler;
}

} // server
} // network
} // flat

#endif // FLAT_NETWORK_SERVER_NETWORKHUB_H


