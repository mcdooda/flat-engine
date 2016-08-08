#ifndef FLAT_LUA_COROUTINE_H
#define FLAT_LUA_COROUTINE_H

#include <lua5.2/lua.hpp>

namespace flat
{
namespace lua
{
namespace coroutine
{

int createCoroutineShortcuts(lua_State* L);
int destroyCoroutineShortcuts(lua_State* L);

enum class CoroutineStatus
{
	RUNNING,
	SUSPENDED,
	NORMAL,
	DEAD
};

// pops the function at the top of the stack and pushes the created coroutine
void create(lua_State* L);

// resumes the coroutine at the top of the stack,
// pops the "numArguments" arguments from the top of the stack
// and pushes "numResults" results from the underlying coroutine.yield
void resume(lua_State* L, int numArguments, int numResults);

// returns the current status of the coroutine at the top of the stack
CoroutineStatus status(lua_State* L, int index);

// private
CoroutineStatus statusFromString(const char* status);


} // coroutine
} // lua
} // flat

#endif // FLAT_LUA_COROUTINE_H



