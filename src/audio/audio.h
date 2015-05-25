#ifndef FLAT_AUDIO_AUDIO_H
#define FLAT_AUDIO_AUDIO_H

#include <string>
#include "music.h"

namespace flat
{
namespace audio
{

class Audio
{
	public:
		Audio();
		~Audio();
		
		Music* loadMusic(const std::string& filename);
};

} // audio
} // flat

#endif // FLAT_AUDIO_AUDIO_H


