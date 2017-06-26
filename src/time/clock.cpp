#include "clock.h"

namespace flat
{
namespace time
{

Clock::Clock()
{
	restart();
}

void Clock::restart()
{
	m_paused = false;
	m_time = 0.f;
	m_dt = 0.f;
	m_dtModifier = 1.f;
}

void Clock::pause()
{
	m_paused = true;
}

void Clock::resume()
{
	m_paused = false;
}

void Clock::update(float dt)
{
	if (!m_paused)
	{
		m_dt = dt * m_dtModifier;
		m_time += m_dt;
	}
}

} // time
} // flat



