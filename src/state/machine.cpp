#include <cstdlib>
#include "machine.h"

namespace flat
{
namespace state
{

Machine::Machine(Agent* agent)
{
	m_agent = agent;
	m_currentState = NULL;
	m_previousState = NULL;
	m_globalState = NULL;
}

Machine::~Machine()
{
	if (m_globalState != NULL)
	{
		m_globalState->exit(m_agent);
		delete m_globalState;
	}

	if (m_previousState != NULL)
		delete m_previousState;

	if (m_currentState != NULL)
		delete m_currentState;
}

void Machine::setState(State* state)
{
	if (m_currentState != NULL)
	{
		m_currentState->exit(m_agent);
		
		if (m_previousState != NULL)
			delete m_previousState;
		
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
	if (m_globalState != NULL)
	{
		m_globalState->exit(m_agent);
		delete m_globalState;
	}

	m_globalState = globalState;
	globalState->enter(m_agent);
}

void Machine::update()
{
	if (m_globalState != NULL)
		m_globalState->execute(m_agent);

	if (m_currentState != NULL)
		m_currentState->execute(m_agent);
}

} // state
} // flat


