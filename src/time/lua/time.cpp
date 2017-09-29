#include <lua5.3/lua.hpp>
#include "time.h"
#include "../time.h"
#include "../../lua/lua.h"
#include "../../flat/game.h"

namespace flat
{
namespace time
{
namespace lua
{

int open(lua_State* L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	static const luaL_Reg Time_lib_s[] = {
		{"getFrameRate", l_Time_getFrameRate},
		
		{nullptr, nullptr}
	};
	luaL_newlib(L, Time_lib_s);
	lua_setglobal(L, "Time");
	
	return 0;
}

int l_Time_getFrameRate(lua_State* L)
{
	const auto& time = flat::lua::getFlat(L).time;
	lua_pushnumber(L, time->getActualFrameRate());
	return 1;
}

} // lua
} // time
} // flat



