#include "audio/music.h"

#include "time/clock.h"
#include "memory/memory.h"
#include "debug/assert.h"

namespace flat
{
namespace audio
{

const Music* Music::currentMusic = nullptr;

Music::Music(const std::string& filename)
{
	m_music = Mix_LoadMUS(filename.c_str());
	FLAT_ASSERT_MSG(false, "TODO: initialize m_clock");
}

Music::~Music()
{
	Mix_FreeMusic(m_music);
	m_music = nullptr;
}

void Music::play(int loops) const
{
	Mix_PlayMusic(m_music, loops);
	m_clock->restart();
	currentMusic = this;
}

void Music::stop() const
{
	FLAT_ASSERT(currentMusic == this);
	Mix_HaltMusic();
	m_clock->pause();
	currentMusic = nullptr;
}

float Music::getPosition() const
{
	return m_clock->getTime();
}

bool Music::move(double delta)
{
	return setPosition(m_clock->getTime() + delta);
}

bool Music::setPosition(double position)
{
	Mix_MusicType musicType = Mix_GetMusicType(m_music);
	bool moved = false;
	time::Clock* clock = m_clock.get();
	const float currentPosition = clock->getTime();

	switch (musicType)
	{
	case MUS_MP3:
	case MUS_MP3_MAD:
		{
			if (position > currentPosition)
			{
				moved = Mix_SetMusicPosition(position - currentPosition) == 0;
			}
			else
			{
				rewind();
				moved = Mix_SetMusicPosition(position) == 0;
			}
		}
		break;
	case MUS_OGG:
		{
			moved = Mix_SetMusicPosition(position) == 0;
		}
		break;
	default:
		break;
	}

	if (moved)
	{
		clock->setTime(static_cast<float>(position));
	}

	return moved;
}

void Music::rewind()
{
	FLAT_ASSERT(currentMusic == this);
	Mix_RewindMusic();
	m_clock->pause();
}

void Music::pause()
{
	FLAT_ASSERT(currentMusic == this);
	Mix_PauseMusic();
	m_clock->pause();
}

void Music::resume()
{
	FLAT_ASSERT(currentMusic == this);
	Mix_ResumeMusic();
	m_clock->resume();
}

bool Music::isPaused() const
{
	return m_clock->getDT() == 0.f;
}

void Music::togglePause()
{
	if (isPaused())
		resume();

	else
		pause();
}

} // audio
} // flat


