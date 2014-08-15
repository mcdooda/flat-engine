#include "music.h"

namespace engine
{
namespace audio
{

Music::Music(std::string filename)
{
	m_music = Mix_LoadMUS(filename.c_str());
}

Music::~Music()
{
	Mix_FreeMusic(m_music);
}

void Music::play()
{
	Mix_PlayMusic(m_music, 1);
}

} // audio
} // engine


