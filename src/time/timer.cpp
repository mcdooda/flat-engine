#include "timer.h"

namespace flat
{
namespace timer
{

}

void time::Timer::start()
{
	m_timePaused = false;
	setTime(0.0f);
}

void time::Timer::stop()
{
	pause();
}

void time::Timer::setTime(float time)
{
	m_pauseElapsedTime = getRealTime() - time;
}

// timer
} // flat
