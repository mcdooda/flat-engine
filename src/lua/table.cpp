#include <lua5.3/lua.hpp>

#include "lua/table.h"
#include "lua/debug.h"

namespace flat
{
namespace lua
{
namespace table
{

void clear(lua_State* L, int index)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	index = lua_absindex(L, index);

	lua_pushnil(L);
	while (lua_next(L, index) != 0)
	{
		const char* modname = luaL_checkstring(L, -2);
		lua_pop(L, 1); // pop value
		lua_pushnil(L);
		lua_setfield(L, index, modname); // clear field
		lua_pop(L, 1);
		lua_pushnil(L);
	}
}

} // table
} // lua
} // flat