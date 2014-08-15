#ifndef FLAT_STATE_AGENT_H
#define FLAT_STATE_AGENT_H

#include "machine.h"

namespace flat
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
} // flat

#endif // FLAT_STATE_AGENT_H


