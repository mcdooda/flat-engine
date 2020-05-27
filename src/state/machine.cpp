#include <cstdlib>

#include "state/machine.h"
#include "state/state.h"

#include "memory/memory.h"

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

void Machine::setState(std::unique_ptr<State>&& state)
{
	if (m_currentState)
	{
		m_currentState->exit(m_agent);
	}

	if (state != nullptr)
	{
		m_currentState = std::move(state);
		m_currentState->enter(m_agent);
	}
	else
	{
		m_currentState = nullptr;
	}
}

void Machine::setNextState(std::unique_ptr<State>&& state)
{
	m_nextState = std::move(state);
}

void Machine::update()
{
	if (m_nextState)
	{
		setState(std::move(m_nextState));
	}

	if (m_currentState)
	{
		m_currentState->execute(m_agent);
	}
}

} // state
} // flat


