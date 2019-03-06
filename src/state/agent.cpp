#include "state/agent.h"

#include "memory/memory.h"

namespace flat
{
namespace state
{

Agent::Agent() :
	m_machine(*this)
{

}

Agent::~Agent()
{

}

} // state
} // flat


