#ifndef FLAT_LUA_TIMER_TIMER_H
#define FLAT_LUA_TIMER_TIMER_H

#include "lua/sharedluareference.h"

namespace flat
{
namespace lua
{
namespace timer
{
class TimerContainer;

class Timer
{
	public:
		Timer() = delete;
		Timer(TimerContainer* timerContainer);
		Timer(const Timer&) = delete;
		Timer(Timer&&) = delete;
		~Timer() = default;

		void operator=(const Timer&) = delete;
		void operator=(Timer&&) = delete;
		
		inline void setBeginTime(float beginTime) const { m_beginTime = beginTime; }
		inline float getBeginTime() const { return m_beginTime; }

		inline void setOnUpdate(const flat::lua::SharedLuaReference<LUA_TFUNCTION>& onUpdate) { m_onUpdate = onUpdate; }
		inline void setOnEnd(const flat::lua::SharedLuaReference<LUA_TFUNCTION>& onEnd) { m_onEnd = onEnd; }

		inline void setDuration(float duration) { m_duration = duration; }
		inline float getDuration() const { return m_duration; }
		inline float getTimeOut() const { return m_beginTime + m_duration; }

		inline const flat::lua::SharedLuaReference<LUA_TFUNCTION>& getOnUpdate() const { return m_onUpdate; }
		inline const flat::lua::SharedLuaReference<LUA_TFUNCTION>& getOnEnd() const { return m_onEnd; }

		inline void setLoop(bool loop) { m_loop = loop; }
		inline bool getLoop() const { return m_loop; }
		
		float getElapsedTime() const;
		inline bool isFinished(float currentTime) const { return currentTime >= getTimeOut(); }

		inline TimerContainer& getTimerContainer() const { return *m_timerContainer; }

		bool stop();
		
	private:
		TimerContainer* m_timerContainer;
		flat::lua::SharedLuaReference<LUA_TFUNCTION> m_onUpdate;
		flat::lua::SharedLuaReference<LUA_TFUNCTION> m_onEnd;
		mutable float m_beginTime;
		float m_duration;
		bool m_loop;
};

} // timer
} // lua
} // flat

#endif // FLAT_LUA_TIMER_TIMER_H



