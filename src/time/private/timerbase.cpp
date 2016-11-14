#include <SDL2/SDL.h>
#include "timerbase.h"
#include "../../debug/assert.h"

namespace flat
{
namespace time
{
namespace private_
{

TimerBase::TimerBase() :
	m_pauseElapsedTime(0.0f),
	m_timePaused(false)
{

}

void TimerBase::pause()
{
	FLAT_ASSERT(!m_timePaused);
	m_pauseTime = getTime();
	m_pauseRealTime = getRealTime();
	m_timePaused = true;
}

void TimerBase::resume()
{
	FLAT_ASSERT(m_timePaused);
	m_timePaused = false;
	m_pauseElapsedTime += getRealTime() - m_pauseRealTime;
}

void TimerBase::togglePause()
{
	if (m_timePaused)
		resume();

	else
		pause();
}

float TimerBase::getTime() const
{
	return m_timePaused ? m_pauseTime : getRealTime() - m_pauseElapsedTime;
}

float TimerBase::getRealTime()
{
	return SDL_GetTicks() / 1000.f;
}

}
}
}
