#include <lua5.3/lua.hpp>

#include "profiler/lua/profiler.h"
#include "profiler/profiler.h"

#include "lua/debug.h"

namespace flat
{
namespace profiler
{
namespace lua
{

int open(lua_State* L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	static const luaL_Reg flat_profiler_lib_s[] = {
		{"start", l_flat_profiler_start },
		{"stop",  l_flat_profiler_stop },

		{nullptr, nullptr}
	};

	lua_getglobal(L, "flat");
	luaL_newlib(L, flat_profiler_lib_s);
	lua_setfield(L, -2, "profiler");

	lua_pop(L, 1);

	return 0;
}

int l_flat_profiler_start(lua_State* L)
{
	Profiler::getInstance().startRecording();
	return 0;
}

int l_flat_profiler_stop(lua_State* L)
{
	Profiler::getInstance().stopRecording();
	return 0;
}

} // lua
} // profiler
} // flat


