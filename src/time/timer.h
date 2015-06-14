#ifndef FLAT_TIME_TIMER_H
#define FLAT_TIME_TIMER_H

#include "private/timerbase.h"

namespace flat
{
namespace time
{

class Timer : public private_::TimerBase
{
public:
	void start();
	void stop();
	void setTime(float time);
};

} // time
} // flat

#endif // FLAT_TIME_TIMER_H


