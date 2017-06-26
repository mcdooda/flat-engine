#ifndef FLAT_TIME_CLOCK_H
#define FLAT_TIME_CLOCK_H

namespace flat
{
namespace time
{

class Clock
{
	public:
		Clock();
		Clock(const Clock&) = delete;
		Clock(Clock&&) = delete;
		~Clock() = default;

		void operator=(const Clock&) = delete;

		void restart();

		void pause();
		void resume();

		void update(float dt);

		inline float getDT() const { return m_paused ? 0.f : m_dt; }

		inline void setDTModifier(float dtModifier) { m_dtModifier = dtModifier; }
		inline float getDTModifier() const { return m_dtModifier; }

		inline void setTime(float time) { m_time = time; }
		inline float getTime() const { return m_time; }

	private:
		float m_time;
		float m_dt;
		float m_dtModifier;
		bool m_paused;
};

} // time
} // flat

#endif // FLAT_TIME_CLOCK_H


