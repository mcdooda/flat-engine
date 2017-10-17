#ifndef FLAT_NETWORK_SERVER_NETWORKHUB_H
#define FLAT_NETWORK_SERVER_NETWORKHUB_H

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

	void send(const common::NetworkEvent& event, Client& client);
};

} // server
} // network
} // flat

#endif // FLAT_NETWORK_SERVER_NETWORKHUB_H


