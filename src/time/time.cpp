#include <algorithm>
#include <SDL2/SDL.h>
#include "time.h"

namespace flat
{
namespace time
{

Time::Time() : private_::TimerBase()
{
	m_frameTime = 0.0f;
	setFrameRate(60.0f);
}

void Time::beginFrame()
{
	m_beginFrameTime = getRealTime();
}

void Time::endFrame()
{
	float endFrameTime = getRealTime();
	m_frameTime = endFrameTime - m_beginFrameTime;
	if (m_frameDuration > 0.0f && m_frameTime < m_frameDuration)
	{
		sleep(m_frameDuration - m_frameTime);
		m_frameTime = m_frameDuration;
	}
}

void Time::setFrameRate(float rate)
{
	m_frameDuration = 1.f / rate;
}

float Time::getFrameRate()
{
	return 1.f / m_frameDuration;
}

void Time::setNoLimitFrameRate()
{
	m_frameDuration = 0.0f;
}

float Time::getFrameTime()
{
	return m_timePaused ? 0.0f : m_frameTime;
}

float Time::getActualFrameRate()
{
	return std::min(1.f / m_frameTime, getFrameRate());
}

void Time::sleep(float duration)
{
	SDL_Delay(duration * 1000.0f);
}

} // time
} // flat


