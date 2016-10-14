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
		inline T& as()
		{
			return static_cast<T&>(*this);
		}

		template <class T>
		inline const T& as() const
		{
			return static_cast<const T&>(*this);
		}

		// "to" = safe "as"
		template <class T>
		inline T& to()
		{
			FLAT_ASSERT(dynamic_cast<T*>(this) != nullptr);
			return as<T>();
		}
		
		template <class T>
		inline const T& to() const
		{
			FLAT_ASSERT(dynamic_cast<const T>(this) != nullptr);
			return as<T>();
		}

	private:
		Machine m_machine;
};

} // state
} // flat

#endif // FLAT_STATE_AGENT_H


