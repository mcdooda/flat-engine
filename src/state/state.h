#ifndef FLAT_STATE_STATE_H
#define FLAT_STATE_STATE_H

#include "agent.h"

namespace flat
{
namespace state
{

class State : public util::Convertible<State>
{
	public:
		State();
		virtual ~State();

		virtual void enter(Agent& agent) = 0;
		virtual void execute(Agent& agent) = 0;
		virtual void exit(Agent& agent) = 0;
};

template <class T>
class StateImpl : public State
{
	public:
		void enter(Agent& agent) override final;
		void execute(Agent& agent) override final;
		void exit(Agent& agent) override final;

		virtual void enter(T& agent) = 0;
		virtual void execute(T& agent) = 0;
		virtual void exit(T& agent) = 0;
};

template<class T>
void StateImpl<T>::enter(Agent& agent)
{
	enter(agent.to<T>());
}

template<class T>
void StateImpl<T>::execute(Agent& agent)
{
	execute(agent.to<T>());
}

template<class T>
void StateImpl<T>::exit(Agent& agent)
{
	exit(agent.as<T>());
}

} // state
} // flat

#endif // FLAT_STATE_STATE_H


