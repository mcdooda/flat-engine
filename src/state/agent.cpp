#include "agent.h"

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
	delete m_machine;
}

} // state
} // flat


