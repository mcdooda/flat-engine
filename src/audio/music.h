#ifndef FLAT_AUDIO_MUSIC_H
#define FLAT_AUDIO_MUSIC_H

#include <memory>
#include <string>
#include <SDL2/SDL_mixer.h>

namespace flat
{
namespace time
{
class Clock;
}

namespace audio
{

class Music
{
	public:
		Music(const std::string& filename);
		~Music();

		void play(int loops = -1) const;
		void stop() const;

		float getPosition() const;

		bool move(double delta);
		bool setPosition(double position);
		void rewind();

		void pause();
		void resume();
		bool isPaused() const;
		void togglePause();

	private:
		Mix_Music* m_music;
		std::shared_ptr<time::Clock> m_clock;
};

} // audio
} // flat

#endif // FLAT_AUDIO_MUSIC_H


