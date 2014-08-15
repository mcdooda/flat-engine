#ifndef ENGINE_STATE_STATE_H
#define ENGINE_STATE_STATE_H

#include "agent.h"

namespace engine
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
} // engine

#endif // ENGINE_STATE_STATE_H


