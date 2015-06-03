#include "music.h"

namespace flat
{
namespace audio
{

Music::Music(const std::string& filename)
{
	m_music = Mix_LoadMUS(filename.c_str());
}

Music::~Music()
{
	Mix_FreeMusic(m_music);
}

void Music::play(int loops) const
{
	Mix_PlayMusic(m_music, loops);
}

void Music::pause()
{
	Mix_PauseMusic();
}

void Music::resume()
{
	Mix_ResumeMusic();
}

} // audio
} // flat


