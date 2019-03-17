#ifndef FLAT_STATE_AGENT_H
#define FLAT_STATE_AGENT_H

#include "state/machine.h"

#include "util/convertible.h"

namespace flat
{
namespace state
{

class Agent : public util::Convertible<Agent>
{
	public:
		Agent();
		virtual ~Agent();

		inline Machine& getStateMachine() { return m_machine; }
		inline const Machine& getStateMachine() const { return m_machine; }

	private:
		Machine m_machine;
};

} // state
} // flat

#endif // FLAT_STATE_AGENT_H


