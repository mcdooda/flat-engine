#include <cstdlib>
#include "machine.h"

#include "state.h"
#include "../memory/memory.h"

namespace flat
{
namespace state
{

Machine::Machine(Agent& agent) :
	m_agent(agent)
{
	
}

Machine::~Machine()
{
	if (m_currentState)
	{
		m_currentState->exit(m_agent);
	}
}

void Machine::setState(State* state)
{
	if (m_currentState)
	{
		m_currentState->exit(m_agent);
	}
	m_currentState.reset(state);
	state->enter(m_agent);
}

void Machine::update()
{
	if (m_currentState)
	{
		m_currentState->execute(m_agent);
	}
}

} // state
} // flat


