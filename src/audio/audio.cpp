#include <SDL2/SDL_mixer.h>
#include "audio.h"

namespace flat
{
namespace audio
{

Audio::Audio()
{
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);
}

Audio::~Audio()
{
	Mix_CloseAudio();
}

Music* Audio::loadMusic(const std::string& filename)
{
	return new Music(filename);
}

} // audio
} // flat


