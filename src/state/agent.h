#ifndef ENGINE_STATE_AGENT_H
#define ENGINE_STATE_AGENT_H

#include "machine.h"

namespace engine
{
namespace state
{

class Machine;

class Agent
{
	public:
		Agent();
		~Agent();

		inline Machine* getStateMachine() const { return m_machine; }

	private:
		Machine* m_machine;
};

} // state
} // engine

#endif // ENGINE_STATE_AGENT_H


