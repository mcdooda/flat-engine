#ifndef AUDIO_AUDIO_H
#define AUDIO_AUDIO_H

#include <string>
#include "music.h"

namespace engine
{
namespace audio
{

class Audio
{
	public:
		Audio();
		~Audio();
		
		Music* loadMusic(std::string filename);
};

} // audio
} // engine

#endif // AUDIO_AUDIO_H


