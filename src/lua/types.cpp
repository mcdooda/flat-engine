#include "lua/types.h"
#include "lua/debug.h"
#include "lua/lua.h"

#include "flat.h"
#include "debug/assert.h"

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

	lua_pushcfunction(L, l_flat_typetostring);
	lua_setfield(L, -2, "typetostring");

	lua_newtable(L);
	registerNativeType(L, LUA_TNIL, "NIL");
	registerNativeType(L, LUA_TBOOLEAN, "BOOLEAN");
	registerNativeType(L, LUA_TLIGHTUSERDATA, "LIGHTUSERDATA");
	registerNativeType(L, LUA_TNUMBER, "NUMBER");
	registerNativeType(L, LUA_TSTRING, "STRING");
	registerNativeType(L, LUA_TTABLE, "TABLE");
	registerNativeType(L, LUA_TFUNCTION, "FUNCTION");
	registerNativeType(L, LUA_TUSERDATA, "USERDATA");
	registerNativeType(L, LUA_TTHREAD, "THREAD");
	lua_setfield(L, -2, "types");

	lua_pop(L, 1);

	return 0;
}

void registerNativeType(lua_State* L, int type, const char* typeName)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	luaL_checktype(L, -1, LUA_TTABLE); // the flat.types table is supposed to be at the top of the stack
	lua_pushinteger(L, type);
	lua_setfield(L, -2, typeName);
}

void registerType(lua_State* L, size_t newTypeHash, const char* typeName)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	lua_getglobal(L, "flat");
	lua_getfield(L, -1, "types");
	lua_pushinteger(L, newTypeHash);
	lua_setfield(L, -2, typeName);
	lua_pop(L, 2);
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

int l_flat_typetostring(lua_State* L)
{
	size_t type = static_cast<size_t>(luaL_checkinteger(L, 1));
	if (type >= 0 && type < LUA_NUMTAGS)
	{
		const char* typeName = lua_typename(L, static_cast<int>(type));
		lua_pushstring(L, typeName);
		return 1;
	}
	Flat& flat = lua::getFlat(L);
	const char* typeName = flat.lua->getTypeName(type);
	if (typeName != nullptr)
	{
		lua_pushstring(L, typeName);
	}
	else
	{
		lua_pushfstring(L, "(Unknown type %d)", type);
	}
	return 1;
}

} // types
} // lua
} // flat


