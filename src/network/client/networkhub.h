#ifndef FLAT_NETWORK_CLIENT_NETWORKHUB_H
#define FLAT_NETWORK_CLIENT_NETWORKHUB_H

#include "server.h"

#include "../common/networkhub.h"

namespace flat
{
namespace network
{
namespace client
{

class NetworkHub : public common::NetworkHub
{
	public:
		NetworkHub();

		void send(const common::NetworkEvent& event);

	private:
		Server m_server;
};

} // client
} // network
} // flat

#endif // FLAT_NETWORK_CLIENT_NETWORKHUB_H


