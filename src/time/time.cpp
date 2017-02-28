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
	setFrameRate(600.0f);
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

float Time::getFrameRate() const
{
	return 1.f / m_frameDuration;
}

void Time::setNoLimitFrameRate()
{
	m_frameDuration = 0.0f;
}

float Time::getFrameTime() const
{
	return m_timePaused ? 0.0f : m_frameTime;
}

float Time::getActualFrameTime() const
{
	return m_frameTime;
}

float Time::getActualFrameRate() const
{
	return m_frameTime > 0.f ? std::min(1.f / m_frameTime, getFrameRate()) : getFrameRate();
}

void Time::sleep(float duration) const
{
	SDL_Delay(static_cast<Uint32>(duration * 1000.f));
}

} // time
} // flat


