#include "networkevent.h"
#include "../../debug/assert.h"

namespace flat
{
namespace network
{
namespace common
{

NetworkEvent::Delimiter NetworkEvent::delimiter = 0;

void NetworkEvent::setDelimiter(Delimiter delimiter)
{
	FLAT_ASSERT(delimiter != 0 && NetworkEvent::delimiter == 0);
	NetworkEvent::delimiter = delimiter;
}

NetworkEvent::Delimiter NetworkEvent::getDelimiter()
{
	FLAT_ASSERT(delimiter != 0);
	return delimiter;
}

} // common
} // network
} // flat

