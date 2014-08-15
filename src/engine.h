#ifndef ENGINE_ENGINE_H
#define ENGINE_ENGINE_H

#include "input/input.h"
#include "time/time.h"
#include "video/video.h"
#include "audio/audio.h"
#include "random/random.h"

namespace engine
{

class Engine
{
	public:
		Engine();
		virtual ~Engine();
		
	public:
		input::Input* input;
		time::Time* time;
		video::Video* video;
		audio::Audio* audio;
		random::Random* random;
};

} // engine

#endif // ENGINE_ENGINE_H


