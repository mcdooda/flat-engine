#ifndef FLAT_LUA_TIMER_TIMERCONTAINER_H
#define FLAT_LUA_TIMER_TIMERCONTAINER_H

#include <memory>
#include <deque>
#include <vector>

#include "lua/timer/timer.h"

#include "containers/pool.h"
#include "time/clock.h"
#include "debug/assert.h"

namespace flat
{
namespace time
{
class Clock;
}
namespace lua
{
namespace timer
{

class TimerContainer
{
	public:
		TimerContainer() = delete;
		TimerContainer(const std::shared_ptr<time::Clock>& clock);
		TimerContainer(const TimerContainer&) = delete;
		TimerContainer(TimerContainer&&) = delete;
		~TimerContainer();

		void operator=(const TimerContainer&) = delete;

		size_t getNumTimers() const;
		size_t getNumFrameTimers() const;

		inline const time::Clock& getClock() const { return *m_clock; }
		
		Timer* add();
		bool stop(Timer* timer);
		
		void updateTimers(lua_State* L);
		
		void clearTimers();

		static void callTimerUpdate(lua_State* L, Timer* timer);
		static void callTimerEnd(lua_State* L, Timer* timer);

	private:
		static bool compareTimersByTimeout(const Timer* a, const Timer* b);


	private:
		containers::Pool<Timer, 256> m_timerPool;
		std::vector<Timer*> m_pendingTimers;
		std::deque<Timer*> m_timers;
		std::vector<Timer*> m_frameTimers;
		std::shared_ptr<time::Clock> m_clock;
};

} // timer
} // lua
} // flat

#endif // FLAT_LUA_TIMER_TIMERCONTAINER_H



