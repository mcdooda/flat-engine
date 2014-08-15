#include "agent.h"

namespace engine
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
} // engine


