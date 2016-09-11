#ifndef FLAT_STATE_MACHINE_H
#define FLAT_STATE_MACHINE_H

#include <utility>
#include "state.h"

namespace flat
{
namespace state
{

class Agent;
class State;

class Machine
{
	public:
		Machine(Agent* agent);
		~Machine();

		template <typename T, typename... U>
		void setNewState(U&... params)
		{
			setState(new T(params...));
		}

		template <typename T, typename... U>
		void setNewStateMove(U&&... params)
		{
			setState(new T(std::move(params)...));
		}

		void setState(State* state);
		void revertToPreviousState();
		void setGlobalState(State* globalState);

		inline State* getCurrentState() const { return m_currentState; }

		void update();

	private:
		State* m_currentState;
		State* m_previousState;
		State* m_globalState;
		Agent* m_agent;
};

} // state
} // flat

#endif // FLAT_STATE_MACHINE_H


