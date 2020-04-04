#include <SDL2/SDL_mixer.h>

#include "audio/audio.h"
#include <iostream>

namespace flat
{
namespace audio
{

const Music* Audio::currentMusic = nullptr;

Audio::Audio()
{
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);
	Mix_AllocateChannels(10);
	Mix_Volume(1, MIX_MAX_VOLUME/2);
}

Audio::~Audio()
{
	Mix_CloseAudio();
}

void Audio::setVolume(float volume) const
{
	Mix_Volume(-1, volume * 128);
}

} // audio
} // flat
