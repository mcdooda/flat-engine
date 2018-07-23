#ifndef FLAT_FLAT_H
#define FLAT_FLAT_H

#include <memory>

// single header public API

#include "memory/memory.h"

#include "input/input.h"
#include "input/lua/mouse.h"

// time
#include "time/time.h"
#include "time/clock.h"
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
#include "geometry/grid.h"
#include "geometry/bezier.h"

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
#include "lua/slotproxy.h"
#include "lua/table.h"
#include "lua/thread.h"
#include "lua/timer/lua/timer.h"

// render
#include "render/animatedsprite.h"
#include "render/synchronizedsprite.h"
#include "render/spritesynchronizer.h"
#include "render/heightmap.h"
#include "render/spritebatch.h"
#include "render/programsettings.h"

// containers
#include "containers/pool.h"
#include "containers/dynamicpool.h"

// resource
#include "resource/sharedresourcemanager.h"
#include "resource/weakresourcemanager.h"

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
#include "misc/lua/vector2.h"
#include "misc/lua/vector3.h"

// debug
#include "debug/helpers.h"

// profiler
#include "profiler/profiler.h"
#include "profiler/profilersection.h"

namespace flat
{

class Flat
{
	public:
		Flat(const std::string& luaPath, const std::string& assetsPath);
		Flat(const Flat&) = delete;
		Flat(Flat&&) = delete;
		virtual ~Flat();
		Flat& operator=(const Flat&) = delete;
		
	public:
		std::unique_ptr<time::Time> time;
		std::unique_ptr<video::Video> video;
		std::unique_ptr<audio::Audio> audio;
		std::unique_ptr<input::Input> input;
		std::unique_ptr<random::Random> random;
		std::unique_ptr<sharp::ui::Ui> ui;
		std::unique_ptr<lua::Lua> lua;
};

} // flat

#endif // FLAT_FLAT_H


