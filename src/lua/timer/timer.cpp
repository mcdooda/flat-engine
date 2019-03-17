#include "lua/timer/timer.h"
#include "lua/timer/timercontainer.h"

namespace flat
{
namespace lua
{
namespace timer
{

Timer::Timer(TimerContainer* timerContainer) :
	m_timerContainer(timerContainer),
	m_beginTime(-1.f),
	m_duration(0.f),
	m_loop(false)
{
	
}

float Timer::getElapsedTime() const
{
	const float currentTime = m_timerContainer->getClock().getTime();
	return currentTime - m_beginTime;
}

bool Timer::stop()
{
	return m_timerContainer->stop(this);
}

} // timer
} // lua
} // flat
