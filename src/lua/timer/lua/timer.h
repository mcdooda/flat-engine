#ifndef FLAT_TIME_TIMER_LUA_TIMER_H
#define FLAT_TIME_TIMER_LUA_TIMER_H

struct lua_State;

namespace flat
{
class Flat;
namespace lua
{
class Lua;
namespace timer
{
class Timer;
class TimerContainer;
namespace lua
{

int open(Lua& lua);

int l_Timer(lua_State* L);
int l_Timer_start(lua_State* L);
int l_Timer_stop(lua_State* L);
int l_Timer_getElapsedTime(lua_State* L);
int l_Timer_onUpdate(lua_State* L);
int l_Timer_onEnd(lua_State* L);

// private
Timer* getTimer(lua_State* L, int index);
void pushTimer(lua_State* L, Timer* timer);

} // lua
} // timer
} // lua
} // flat

#endif // FLAT_TIME_TIMER_LUA_TIMER_H



