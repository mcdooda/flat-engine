#include <lua5.3/lua.hpp>

#include "lua/table.h"
#include "lua/debug.h"

namespace flat::lua::table
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

bool inheritsFrom(lua_State* L, int classIndex, int superClassIndex)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	lua_pushvalue(L, classIndex);
	classIndex = -1;

	superClassIndex = lua_absindex(L, superClassIndex);

	while (true)
	{
		if (lua_rawequal(L, classIndex, superClassIndex))
		{
			lua_pop(L, 1);
			return true;
		}

		// push super class onto the stack and retry unless the metafield is not a table
		const int indexMetaFieldType = luaL_getmetafield(L, classIndex, "__index");
		lua_remove(L, -2);
		if (indexMetaFieldType != LUA_TTABLE)
		{
			if (indexMetaFieldType != LUA_TNIL)
			{
				lua_pop(L, 1);
			}
			return false;
		}
	}

	lua_pop(L, 1);
	return false;
}

bool containsAllFields(lua_State* L, int classIndex, int superClassIndex)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	classIndex = lua_absindex(L, classIndex);
	superClassIndex = lua_absindex(L, superClassIndex);

	lua_pushnil(L);
	while (lua_next(L, superClassIndex) != 0)
	{
		//FLAT_LUA_EXPECT_STACK_GROWTH(L, -1);

		// pop superclass value
		lua_pop(L, 1);

		// push duplicate of the key
		lua_pushvalue(L, -1);

		if (lua_gettable(L, classIndex) == LUA_TNIL)
		{
			lua_pop(L, 2);
			return false;
		}

		lua_pop(L, 1);
	}

	return true;
}

bool implementsInterface(lua_State* L, int objectIndex, int interfaceIndex)
{
	return inheritsFrom(L, objectIndex, interfaceIndex) || containsAllFields(L, objectIndex, interfaceIndex);
}

} // flat::lua::table