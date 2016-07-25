#ifndef FLAT_TIME_TIME_H
#define FLAT_TIME_TIME_H

#include "private/timerbase.h"

namespace flat
{
namespace time
{

class Time final : public private_::TimerBase
{
	public:
		Time();

		void beginFrame();
		void endFrame();
		
		void setFrameRate(float rate);
		float getFrameRate();
		void setNoLimitFrameRate();
		float getFrameTime();
		float getActualFrameRate();

		void sleep(float duration);
		
	private:
		float m_frameTime;
		float m_beginFrameTime;
		float m_frameDuration;
};

} // time
} // flat

#endif // FLAT_TIME_TIME_H


