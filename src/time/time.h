#ifndef FLAT_TIME_H
#define FLAT_TIME_H

namespace flat
{
namespace time
{

class Time
{
	public:
		Time();
		~Time();
		
		void beginFrame();
		void endFrame();
		
		void setFrameRate(float rate);
		float getFrameRate();
		void setNoLimitFrameRate();

		void sleep(float duration);
		float getRealTime();

		void pause();
		void resume();
		inline bool isPaused() const { return m_timePaused; }
		void togglePause();

		float getTime();
		float getFrameTime();
		
	private:
		bool m_timePaused;
		float m_pauseTime;
		float m_pauseRealTime;
		float m_pauseElapsedTime;

		float m_frameTime;
		float m_beginFrameTime;
		float m_frameDuration;
};

} // time
} // flat

#endif // FLAT_TIME_H


