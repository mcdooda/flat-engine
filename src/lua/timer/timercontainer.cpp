#include <algorithm>
#include <lua5.3/lua.hpp>

#include "lua/timer/timercontainer.h"
#include "lua/timer.h"
#include "lua/timer/lua/timer.h"

#include "time/clock.h"
#include "profiler/profilersection.h"

namespace flat
{
namespace lua
{
namespace timer
{

TimerContainer::TimerContainer(const std::shared_ptr<time::Clock>& clock) :
	m_clock(clock)
{
	FLAT_ASSERT(clock != nullptr);
}

TimerContainer::~TimerContainer()
{
	clearTimers();
}

size_t TimerContainer::getNumTimers() const
{
	return m_timers.size() + m_frameTimers.size();
}

size_t TimerContainer::getNumFrameTimers() const
{
	return m_frameTimers.size();
}

Timer* TimerContainer::add()
{
	Timer* timer = m_timerPool.create(this);
	FLAT_ASSERT(timer != nullptr);
	m_pendingTimers.push_back(timer);
	return timer;
}

bool TimerContainer::stop(Timer* timer)
{
	if (!timer->getOnUpdate().isEmpty())
	{
		std::vector<Timer*>::iterator it = std::find(m_frameTimers.begin(), m_frameTimers.end(), timer);
		if (it != m_frameTimers.end())
		{
			m_timerPool.destroy(*it);
			*it = nullptr;
			return true;
		}
		return false;
	}
	else
	{
		std::deque<Timer*>::iterator it = std::find(m_timers.begin(), m_timers.end(), timer);
		if (it != m_timers.end())
		{
			m_timerPool.destroy(*it);
			*it = nullptr;
			return true;
		}
		else
		{
			std::vector<Timer*>::iterator it = std::find(m_pendingTimers.begin(), m_pendingTimers.end(), timer);
			if (it != m_pendingTimers.end())
			{
				m_timerPool.destroy(*it);
				m_pendingTimers.erase(it);
				return true;
			}
			return false;
		}
	}
}

void TimerContainer::updateTimers(lua_State* L)
{
	FLAT_PROFILE("Timer container update");

	FLAT_ASSERT(m_clock != nullptr);
	const float time = m_clock->getTime();
	size_t size = m_timers.size();
	m_timers.erase(
	    std::remove(
			m_timers.begin(),
			m_timers.end(),
			nullptr
			),
		m_timers.end()
	);
	FLAT_ASSERT(std::is_sorted(m_timers.begin(), m_timers.end(), &TimerContainer::compareTimersByTimeout));
	// insert pending timers
	for (std::vector<Timer*>::iterator it = m_pendingTimers.begin(); it != m_pendingTimers.end();)
	{
		Timer* timer = *it;
		if (timer->getBeginTime() >= 0)
		{
			if (!timer->getOnUpdate().isEmpty())
			{
				m_frameTimers.push_back(timer);
			}
			else
			{
				std::deque<Timer*>::iterator sortedIterator = std::upper_bound(m_timers.begin(), m_timers.end(), timer, &TimerContainer::compareTimersByTimeout);
				m_timers.insert(sortedIterator, timer);
			}
			it = m_pendingTimers.erase(it);
		}
		else
		{
			++it;
		}
	}

	// update timers
	for (std::deque<Timer*>::iterator it = m_timers.begin(); it != m_timers.end(); it++)
	{
		Timer* timer = *it;
		if (timer != nullptr)
		{
			const float timeOut = timer->getTimeOut();
			if (time >= timeOut)
			{
				callTimerEnd(L, timer);
				if (timer->getLoop())
				{
					timer->setBeginTime(time);
				}
				else
				{
					m_timerPool.destroy(timer);
					*it = nullptr;
				}
			}
			else
			{
				break;
			}
		}
	}
	std::sort(m_timers.begin(), m_timers.end(), &TimerContainer::compareTimersByTimeout);

	// update frame timers
	for (std::vector<Timer*>::iterator it = m_frameTimers.begin(); it != m_frameTimers.end();)
	{
		Timer* timer = *it;
		if (timer != nullptr)
		{
			const float timeOut = timer->getTimeOut();
			if (time >= timeOut)
			{
				// update one last time before dying
				callTimerUpdate(L, timer);
				if (timer->getLoop())
				{
					timer->setBeginTime(time);
					it++;
				}
				else
				{
					callTimerEnd(L, timer);
					m_timerPool.destroy(timer);
					it = m_frameTimers.erase(it);
				}
			}
			else
			{
				callTimerUpdate(L, timer);
				it++;
			}
		}
		else
		{
			it = m_frameTimers.erase(it);
		}
	}
}

void TimerContainer::clearTimers()
{
	for (Timer* timer : m_timers)
	{
		if (timer != nullptr)
		{
			m_timerPool.destroy(timer);
		}
	}
	m_timers.clear();

	for (Timer* timer : m_frameTimers)
	{
		if (timer != nullptr)
		{
			m_timerPool.destroy(timer);
		}
	}
	m_frameTimers.clear();

	for (Timer* timer : m_pendingTimers)
	{
		m_timerPool.destroy(timer);
	}
	m_pendingTimers.clear();
}

bool TimerContainer::compareTimersByTimeout(const Timer* a, const Timer* b)
{
	if (a == nullptr)
		return true;
	if (b == nullptr)
		return false;
	return a->getTimeOut() < b->getTimeOut();
}

void TimerContainer::callTimerUpdate(lua_State* L, Timer* timer)
{
	FLAT_PROFILE("Timer update");

	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	const flat::lua::SharedLuaReference<LUA_TFUNCTION>& onUpdate = timer->getOnUpdate();
	if (!onUpdate.isEmpty())
	{
		const float elapsedTime = std::min(timer->getElapsedTime(), timer->getDuration());
		onUpdate.call(timer, elapsedTime);
	}
}

void TimerContainer::callTimerEnd(lua_State* L, Timer* timer)
{
	FLAT_PROFILE("Timer end");

	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	const flat::lua::SharedLuaReference<LUA_TFUNCTION>& onEnd = timer->getOnEnd();
	if (!onEnd.isEmpty())
	{
		onEnd.call(timer);
	}
}

} // timer
} // time
} // flat
