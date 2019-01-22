#ifndef FLAT_LUA_PUSH_H
#define FLAT_LUA_PUSH_H

#include <string>
#include <lua5.3/lua.hpp>

#define FLAT_LUA_PUSHABLE_TYPE(Type, push)						\
namespace flat													\
{																\
namespace lua													\
{																\
	inline void pushValue(lua_State* L, Type value)				\
	{															\
		push;													\
	}															\
}																\
}

FLAT_LUA_PUSHABLE_TYPE(int,					lua_pushinteger(L, value))
FLAT_LUA_PUSHABLE_TYPE(float,				lua_pushnumber(L, value))
FLAT_LUA_PUSHABLE_TYPE(bool,				lua_pushboolean(L, value))
FLAT_LUA_PUSHABLE_TYPE(void*,				lua_pushlightuserdata(L, value))
FLAT_LUA_PUSHABLE_TYPE(const char*,			lua_pushstring(L, value))
FLAT_LUA_PUSHABLE_TYPE(const std::string&,	lua_pushstring(L, value.c_str()))

#endif // FLAT_LUA_PUSH_H


