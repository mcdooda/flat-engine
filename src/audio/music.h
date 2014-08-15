#ifndef FLAT_AUDIO_MUSIC_H
#define FLAT_AUDIO_MUSIC_H

#include <string>
#include <SDL2/SDL_mixer.h>

namespace flat
{
namespace audio
{

class Music
{
	public:
		Music(std::string filename);
		~Music();
		
		void play();
		
	private:
		Mix_Music* m_music;
};

} // audio
} // flat

#endif // FLAT_AUDIO_MUSIC_H


