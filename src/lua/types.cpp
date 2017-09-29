#include "types.h"
#include "debug.h"
#include "lua.h"
#include "../debug/assert.h"
#include "../flat.h"

namespace flat
{
namespace lua
{
namespace types
{
static char typesTableIndex = 'i';

int open(lua_State* L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	lua_getglobal(L, "flat");

	lua_pushcfunction(L, l_flat_type);
	lua_setfield(L, -2, "type");

	lua_pushcfunction(L, l_flat_typename);
	lua_setfield(L, -2, "typename");

	lua_pop(L, 1);

	return 0;
}

int l_flat_type(lua_State* L)
{
	int type = lua_type(L, 1);
	if (type != LUA_TUSERDATA)
	{
		lua_pushinteger(L, type);
		return 1;
	}
	bool hasMetatable = lua_getmetatable(L, 1);
	FLAT_ASSERT(hasMetatable);
	lua_getfield(L, -1, "type");
	return 1;
}

int l_flat_typename(lua_State* L)
{
	int type = static_cast<int>(luaL_checkinteger(L, 1));
	const char* typeName = lua_typename(L, type);
	if (typeName != nullptr)
	{
		lua_pushstring(L, typeName);
		return 1;
	}
	Flat& flat = lua::getFlat(L);
	typeName = flat.lua->getTypeName(type);
	FLAT_ASSERT(typeName != nullptr);
	lua_pushstring(L, typeName);
	return 1;
}

} // types
} // lua
} // flat


