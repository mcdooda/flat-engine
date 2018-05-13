#include <algorithm>
#include <SDL2/SDL.h>
#include "time.h"
#include "clock.h"

namespace flat
{
namespace time
{

Time::Time()
{
	m_frameDuration = 0.f;
	setPreferedFrameRate(600.f);

	defaultClock = newClock();
}

void Time::beginFrame()
{
	m_beginFrameTime = getAbsoluteTime();
}

void Time::endFrame()
{
	float endFrameTime = getAbsoluteTime();
	m_frameDuration = endFrameTime - m_beginFrameTime;
	if (m_preferedFrameDuration > 0.0f && m_frameDuration < m_preferedFrameDuration)
	{
		sleep(m_preferedFrameDuration - m_frameDuration);
		m_frameDuration = m_preferedFrameDuration;
	}
	updateClocks();
}

void Time::setPreferedFrameRate(float rate)
{
	m_preferedFrameDuration = 1.f / rate;
}

float Time::getPreferedFrameRate() const
{
	return m_preferedFrameDuration > 0.f ? 1.f / m_preferedFrameDuration : std::numeric_limits<float>::infinity();
}

void Time::setNoLimitFrameRate()
{
	m_preferedFrameDuration = 0.0f;
}

float Time::getActualFrameRate() const
{
	return m_frameDuration > 0.f ? std::min(1.f / m_frameDuration, getPreferedFrameRate()) : getPreferedFrameRate();
}

std::shared_ptr<Clock> Time::newClock()
{
	std::shared_ptr<Clock> clock = std::make_shared<Clock>();
	m_clocks.emplace_back(clock);
	return clock;
}


void Time::updateClocks()
{
	for (std::vector<std::weak_ptr<Clock>>::iterator it = m_clocks.begin(); it != m_clocks.end(); )
	{
		std::weak_ptr<Clock>& clockWeakPtr = *it;
		if (clockWeakPtr.expired())
		{
			it = m_clocks.erase(it);
		}
		else
		{
			Clock* clock = clockWeakPtr.lock().get();
			clock->update(m_frameDuration);
			++it;
		}
	}
}

void Time::sleep(float duration)
{
	SDL_Delay(static_cast<Uint32>(duration * 1000.f));
}

float Time::getAbsoluteTime()
{
	return SDL_GetTicks() / 1000.f;
}

} // time
} // flat


