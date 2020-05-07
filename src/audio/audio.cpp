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
	Mix_Volume(1, MIX_MAX_VOLUME / 2);
}

Audio::~Audio()
{
	Mix_CloseAudio();
}

void Audio::setVolume(float volume) const
{
	Mix_Volume(-1, static_cast<int>(volume * 128.f));
}

std::shared_ptr<flat::audio::Sample> Audio::playChunk(std::shared_ptr<const Chunk> chunk)
{
	std::shared_ptr<flat::audio::Sample>& sample = m_activeSamples.emplace_back(std::make_shared<Sample>(chunk));
	sample->play(0);
	return sample;
}

void Audio::endFrame()
{
	for (int i = static_cast<int>(m_activeSamples.size()) - 1; i >= 0; --i)
	{
		Sample* sample = m_activeSamples.at(i).get();
		if (!sample->isPlaying())
		{
			m_activeSamples.erase(m_activeSamples.begin() + i);
		}
	}
}

} // audio
} // flat
