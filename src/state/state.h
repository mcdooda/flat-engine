#ifndef FLAT_STATE_STATE_H
#define FLAT_STATE_STATE_H

#include "agent.h"

namespace flat
{
namespace state
{

class Agent;

class State
{
	public:
		State();
		virtual ~State();

		virtual void enter(Agent* agent) = 0;
		virtual void execute(Agent* agent) = 0;
		virtual void exit(Agent* agent) = 0;
};

} // state
} // flat

#endif // FLAT_STATE_STATE_H


