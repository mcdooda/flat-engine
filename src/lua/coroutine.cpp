#include <cstring>
#include "coroutine.h"
#include "lua.h"
#include "../debug/assert.h"

namespace flat
{
namespace lua
{
namespace coroutine
{

static char coroutineCreateIndex = 'c';
static char coroutineResumeIndex = 'r';
static char coroutineStatusIndex = 's';

int createCoroutineShortcuts(lua_State* L)
{
	lua_getglobal(L, "coroutine");
	
	lua_pushlightuserdata(L, &coroutineCreateIndex);
	lua_getfield(L, -2, "create");
	lua_settable(L, LUA_REGISTRYINDEX);
	
	lua_pushlightuserdata(L, &coroutineResumeIndex);
	lua_getfield(L, -2, "resume");
	lua_settable(L, LUA_REGISTRYINDEX);
	
	lua_pushlightuserdata(L, &coroutineStatusIndex);
	lua_getfield(L, -2, "status");
	lua_settable(L, LUA_REGISTRYINDEX);
	
	lua_pop(L, 1);
	return 0;
}

int destroyCoroutineShortcuts(lua_State* L)
{
	lua_pushlightuserdata(L, &coroutineCreateIndex);
	lua_pushnil(L);
	lua_settable(L, LUA_REGISTRYINDEX);
	
	lua_pushlightuserdata(L, &coroutineResumeIndex);
	lua_pushnil(L);
	lua_settable(L, LUA_REGISTRYINDEX);
	
	lua_pushlightuserdata(L, &coroutineStatusIndex);
	lua_pushnil(L);
	lua_settable(L, LUA_REGISTRYINDEX);
	
	return 0;
}

void create(lua_State* L)
{
	luaL_checktype(L, -1, LUA_TFUNCTION);
	lua_pushlightuserdata(L, &coroutineCreateIndex);
	lua_gettable(L, LUA_REGISTRYINDEX);
	lua_pushvalue(L, -2);
	lua_call(L, 1, 1);
	lua_replace(L, -2);
}

void resume(lua_State* L, int numArguments, int numResults)
{
	luaL_checktype(L, -1 - numArguments - 1, LUA_TNIL);
	luaL_checktype(L, -1 - numArguments, LUA_TTHREAD);
	lua_pushlightuserdata(L, &coroutineResumeIndex);
	lua_gettable(L, LUA_REGISTRYINDEX);
	lua_replace(L, -1 - numArguments - 2);
	int top = lua_gettop(L);
	lua_call(L, numArguments + 1, LUA_MULTRET);
	int firstArg = top - numArguments - 1;
	if (!lua_toboolean(L, firstArg))
	{
		lua_error(L);
	}
	lua_settop(L, firstArg + numResults);
}

CoroutineStatus status(lua_State* L, int index)
{
	luaL_checktype(L, index, LUA_TTHREAD);
	if (index < 0)
	{
		index = lua_gettop(L) + index + 1;
	}
	lua_pushlightuserdata(L, &coroutineStatusIndex);
	lua_gettable(L, LUA_REGISTRYINDEX);
	lua_pushvalue(L, index);
	lua_call(L, 1, 1);
	const char* statusString = luaL_checkstring(L, -1);
	CoroutineStatus status = statusFromString(statusString);
	lua_pop(L, 1);
	return status;
}

// private
CoroutineStatus statusFromString(const char* name)
{
	static const struct
	{
		const char* name;
		CoroutineStatus status;
	} stringToEnum[] = {
		{"running",   CoroutineStatus::RUNNING},
		{"suspended", CoroutineStatus::SUSPENDED},
		{"normal",    CoroutineStatus::NORMAL},
		{"dead",      CoroutineStatus::DEAD},
		{nullptr}
	};
	for (int i = 0; stringToEnum[i].name; ++i)
	{
		if (std::strcmp(name, stringToEnum[i].name) == 0)
			return stringToEnum[i].status;
	}
	FLAT_ASSERT_MSG(false, "Unknown coroutine status '%s'", name);
	return CoroutineStatus::RUNNING;
}

} // coroutine
} // lua
} // flat



