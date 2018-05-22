#ifndef FLAT_PROFILER_LUA_PROFILER_H
#define FLAT_PROFILER_LUA_PROFILER_H

#ifdef FLAT_PROFILER_ENABLED

struct lua_State;

namespace flat
{
namespace profiler
{
namespace lua
{

int open(lua_State* L);

int l_flat_profiler_start(lua_State* L);
int l_flat_profiler_stop(lua_State* L);

} // lua
} // profiler
} // flat

#endif // FLAT_PROFILER_ENABLED

#endif // FLAT_PROFILER_LUA_PROFILER_H


