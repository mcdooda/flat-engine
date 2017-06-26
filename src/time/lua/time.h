#ifndef FLAT_TIME_LUA_TIME_H
#define FLAT_TIME_LUA_TIME_H

struct lua_State;

namespace flat
{
namespace time
{
class Time;
namespace lua
{

int open(lua_State* L);

int l_Time_getFrameRate(lua_State* L);

} // lua
} // time
} // flat

#endif // FLAT_TIME_LUA_TIME_H



