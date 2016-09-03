#include <cstdlib>
#include "machine.h"

#include "../memory/memory.h"

namespace flat
{
namespace state
{

Machine::Machine(Agent* agent)
{
	m_agent = agent;
	m_currentState = nullptr;
	m_previousState = nullptr;
	m_globalState = nullptr;
}

Machine::~Machine()
{
	if (m_currentState != nullptr)
	{
		m_currentState->exit(m_agent);
		FLAT_DELETE(m_currentState);
	}
	if (m_globalState != nullptr)
	{
		m_globalState->exit(m_agent);
		FLAT_DELETE(m_globalState);
	}
	FLAT_DELETE(m_previousState);
}

void Machine::setState(State* state)
{
	if (m_currentState != nullptr)
	{
		m_currentState->exit(m_agent);
		FLAT_DELETE(m_previousState);
		m_previousState = m_currentState;
	}
	m_currentState = state;
	state->enter(m_agent);
}

void Machine::revertToPreviousState()
{
	setState(m_previousState);
}

void Machine::setGlobalState(State* globalState)
{
	if (m_globalState != nullptr)
	{
		m_globalState->exit(m_agent);
		FLAT_DELETE(m_globalState);
	}
	m_globalState = globalState;
	globalState->enter(m_agent);
}

void Machine::update()
{
	if (m_globalState != nullptr)
		m_globalState->execute(m_agent);

	if (m_currentState != nullptr)
		m_currentState->execute(m_agent);
}

} // state
} // flat


