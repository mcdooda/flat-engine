#ifndef FLAT_FLAT_H
#define FLAT_FLAT_H

// single header public API

#include "memory/memory.h"

#include "input/input.h"
#include "input/lua/mouse.h"

// time
#include "time/time.h"
#include "time/lua/time.h"

// video
#include "video/video.h"

// audio
#include "audio/audio.h"

// random
#include "random/random.h"

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

// lua
#include "lua/lua.h"
#include "lua/debug.h"
#include "lua/sharedluareference.h"
#include "lua/sharedcppreference.h"

// util
#include "util/animatedsprite.h"
#include "util/heightmap.h"
#include "util/spritebatch.h"

// containers
#include "containers/pool.h"

// sharp
// ui
#include "sharp/ui/widgetfactory.h"
#include "sharp/ui/rootwidget.h"
#include "sharp/ui/textwidget.h"
#include "sharp/ui/lua/ui.h"

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


