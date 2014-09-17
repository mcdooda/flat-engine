#ifndef FLAT_FLAT_H
#define FLAT_FLAT_H

#include "input/input.h"
#include "time/time.h"
#include "video/video.h"
#include "audio/audio.h"
#include "random/random.h"

// single header public API

// geometry
#include "geometry/linesegment.h"
#include "geometry/circle.h"
#include "geometry/rectangle.h"

// video
#include "video/pass.h"
#include "video/render.h"
#include "video/color.h"
#include "video/filetexture.h"

// state
#include "state/state.h"

// thread
#include "thread/thread.h"

// lua
#include "lua/lua.h"

// util
#include "util/heightmap.h"

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


