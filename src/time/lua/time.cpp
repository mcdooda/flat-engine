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
	Time* time = flat::lua::getTime(L);
	lua_pushnumber(L, time->getActualFrameRate());
	return 1;
}

} // lua
} // time
} // flat



