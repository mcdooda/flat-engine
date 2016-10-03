#ifndef FLAT_STATE_AGENT_H
#define FLAT_STATE_AGENT_H

#include "machine.h"

namespace flat
{
namespace state
{

class Agent
{
	public:
		Agent();
		virtual ~Agent();

		inline Machine& getStateMachine() { return m_machine; }
		inline const Machine& getStateMachine() const { return m_machine; }

		template <class T>
		T* to()
		{
			FLAT_ASSERT(dynamic_cast<T*>(this) != nullptr);
			return static_cast<T*>(this);
		}
		
		template <class T>
		const T* to() const
		{
			FLAT_ASSERT(dynamic_cast<const T*>(this));
			return static_cast<const T*>(this);
		}

	private:
		Machine m_machine;
};

} // state
} // flat

#endif // FLAT_STATE_AGENT_H


