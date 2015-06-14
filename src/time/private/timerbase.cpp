#include <SDL2/SDL.h>
#include "timerbase.h"

namespace flat
{
namespace time
{
namespace private_
{

TimerBase::TimerBase() : m_pauseElapsedTime(0.0f), m_timePaused(false)
{

}

void TimerBase::pause()
{
	if (!m_timePaused)
	{
		m_pauseTime = getTime();
		m_pauseRealTime = getRealTime();
		m_timePaused = true;
	}
}

void TimerBase::resume()
{
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

float TimerBase::getTime()
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
