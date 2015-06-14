#ifndef FLAT_TIME_TIMERBASE_H
#define FLAT_TIME_TIMERBASE_H

namespace flat
{
namespace time
{
namespace private_
{

class TimerBase
{
public:
	TimerBase();

	void pause();
	void resume();
	inline bool isPaused() const { return m_timePaused; }
	void togglePause();

	float getTime();

protected:
	static float getRealTime();

protected:
	float m_pauseTime;
	float m_pauseRealTime;
	float m_pauseElapsedTime;
	bool m_timePaused;
};

}
}
}

#endif // FLAT_TIME_TIMERBASE_H


