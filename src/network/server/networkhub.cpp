#include "networkhub.h"
#include "client.h"

namespace flat
{
namespace network
{
namespace server
{

void NetworkHub::send(const common::NetworkEvent& event, Client& client)
{
	common::NetworkHub::send(event, client);
}

} // server
} // network
} // flat