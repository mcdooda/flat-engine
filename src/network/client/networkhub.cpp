#include "networkhub.h"

namespace flat
{
namespace network
{
namespace client
{

void NetworkHub::send(const common::NetworkEvent& event)
{
	common::NetworkHub::send(event, m_server);
}

} // client
} // network
} // flat


