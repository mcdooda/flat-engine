#ifndef FLAT_FLAT_H
#define FLAT_FLAT_H

#include "input/input.h"
#include "time/time.h"
#include "video/video.h"
#include "audio/audio.h"
#include "random/random.h"

namespace flat
{

class Flat
{
	public:
		Flat();
		virtual ~Flat();
		
	public:
		input::Input* input;
		time::Time* time;
		video::Video* video;
		audio::Audio* audio;
		random::Random* random;
};

} // flat

#endif // FLAT_FLAT_H


