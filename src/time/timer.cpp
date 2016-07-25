#include "timer.h"

namespace flat
{
namespace time
{

void Timer::start()
{
	m_timePaused = false;
	setTime(0.0f);
}

void Timer::stop()
{
	pause();
}

void Timer::setTime(float time)
{
	m_pauseElapsedTime = getRealTime() - time;
}

} // time
} // flat



