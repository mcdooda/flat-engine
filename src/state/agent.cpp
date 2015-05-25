#include "agent.h"

#include "../memory/memory.h"

namespace flat
{
namespace state
{

Agent::Agent()
{
	m_machine = new Machine(this);
}

Agent::~Agent()
{
	FLAT_DELETE(m_machine);
}

} // state
} // flat


