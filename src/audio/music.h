#ifndef ENGINE_AUDIO_MUSIC_H
#define ENGINE_AUDIO_MUSIC_H

#include <string>
#include <SDL2/SDL_mixer.h>

namespace engine
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
} // engine

#endif // ENGINE_AUDIO_MUSIC_H


