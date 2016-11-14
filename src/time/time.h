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
		float getFrameRate() const;
		void setNoLimitFrameRate();
		float getFrameTime() const;
		float getActualFrameTime() const;
		float getActualFrameRate() const;

		void sleep(float duration) const;
		
	private:
		float m_frameTime;
		float m_beginFrameTime;
		float m_frameDuration;
};

} // time
} // flat

#endif // FLAT_TIME_TIME_H


