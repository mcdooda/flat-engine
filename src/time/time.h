#ifndef FLAT_TIME_TIME_H
#define FLAT_TIME_TIME_H

#include <vector>
#include <memory>

namespace flat
{
namespace time
{
class Clock;

class Time final
{
	public:
		Time();

		void beginFrame();
		void endFrame();
		
		void setPreferedFrameRate(float rate);
		float getPreferedFrameRate() const;
		void setNoLimitFrameRate();		
		float getActualFrameRate() const;

		std::shared_ptr<Clock> newClock();

	private:
		void updateClocks();

		static void sleep(float duration);
		static float getAbsoluteTime();
		
	private:
		std::vector<std::weak_ptr<Clock>> m_clocks;
		float m_beginFrameTime;
		float m_frameDuration;
		float m_preferedFrameDuration;
};

} // time
} // flat

#endif // FLAT_TIME_TIME_H


