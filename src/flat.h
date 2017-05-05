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
#include "geometry/quadtree.h"

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
#include "lua/uniqueluareference.h"
#include "lua/sharedcppreference.h"
#include "lua/table.h"
#include "lua/thread.h"

// render
#include "render/animatedsprite.h"
#include "render/heightmap.h"
#include "render/spritebatch.h"

// containers
#include "containers/pool.h"
#include "containers/dynamicpool.h"

// sharp
// ui
#include "sharp/ui/ui.h"
#include "sharp/ui/widgetfactory.h"
#include "sharp/ui/rootwidget.h"
#include "sharp/ui/textwidget.h"
#include "sharp/ui/lua/ui.h"

// misc
#include "misc/std.h"
#include "misc/aabb2.h"
#include "misc/aabb3.h"

// debug
#include "debug/helpers.h"

namespace flat
{

class Flat
{
	public:
		Flat();
		virtual ~Flat();
		
	public:
		time::Time* time;
		video::Video* video;
		audio::Audio* audio;
		input::Input* input;
		random::Random* random;
		sharp::ui::Ui* ui;
		lua::Lua* lua;
};

} // flat

#endif // FLAT_FLAT_H


