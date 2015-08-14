#include "music.h"

#include "../time/timer.h"
#include "../memory/memory.h"
#include "../debug/assert.h"

namespace flat
{
namespace audio
{

const Music* Music::currentMusic = nullptr;

Music::Music(const std::string& filename)
{
	m_music = Mix_LoadMUS(filename.c_str());
	m_timer = new time::Timer();
}

Music::~Music()
{
	Mix_FreeMusic(m_music);
	m_music = nullptr;
	FLAT_DELETE(m_timer);
}

void Music::play(int loops) const
{
	Mix_PlayMusic(m_music, loops);
	m_timer->start();
	currentMusic = this;
}

void Music::stop() const
{
	FLAT_ASSERT(currentMusic == this);
	Mix_HaltMusic();
	m_timer->stop();
	currentMusic = nullptr;
}

float Music::getPosition() const
{
	return m_timer->getTime();
}

bool Music::move(double delta)
{
	return setPosition(m_timer->getTime() + delta);
}

bool Music::setPosition(double position)
{
	Mix_MusicType musicType = Mix_GetMusicType(m_music);
	bool moved = false;
	const float currentPosition = m_timer->getTime();

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
		m_timer->setTime(position);

	return moved;
}

void Music::rewind()
{
	FLAT_ASSERT(currentMusic == this);
	Mix_RewindMusic();
	m_timer->stop();
}

void Music::pause()
{
	FLAT_ASSERT(currentMusic == this);
	Mix_PauseMusic();
	m_timer->pause();
}

void Music::resume()
{
	FLAT_ASSERT(currentMusic == this);
	Mix_ResumeMusic();
	m_timer->resume();
}

bool Music::isPaused() const
{
	return m_timer->isPaused();
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


