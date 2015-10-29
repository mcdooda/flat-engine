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

		template <class T>
		T* to()
		{
			FLAT_ASSERT(dynamic_cast<T*>(this));
			return static_cast<T*>(this);
		}

	private:
		Machine* m_machine;
};

} // state
} // flat

#endif // FLAT_STATE_AGENT_H


