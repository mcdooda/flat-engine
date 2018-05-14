#include "timer.h"
#include "../timer.h"
#include "../timercontainer.h"
#include "../../../flat.h"

namespace flat
{
namespace lua
{
namespace timer
{
namespace lua
{

using LuaTimer = flat::lua::SharedCppValue<Timer*>;

int open(Lua& lua)
{
	lua_State* L = lua.state;
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	static const luaL_Reg Timer_lib_m[] = {
		{"start",          l_Timer_start},
		{"stop",           l_Timer_stop},
		{"getElapsedTime", l_Timer_getElapsedTime},
		{"onUpdate",       l_Timer_onUpdate},
		{"onEnd",          l_Timer_onEnd},
		
		{nullptr, nullptr}
	};
	lua.registerClass<LuaTimer>("flat.Timer", Timer_lib_m);
	
	// constructor: flat.Timer()
	lua_getglobal(L, "flat");
	lua_pushcfunction(L, l_Timer);
	lua_setfield(L, -2, "Timer");

	lua_pop(L, 1);
	
	return 0;
}

int l_Timer(lua_State* L)
{
	TimerContainer* timerContainer = flat::lua::getFlat(L).lua->defaultTimerContainer.get();
	pushTimer(L, timerContainer->add());
	return 1;
}

int l_Timer_start(lua_State* L)
{
	Timer* timer = getTimer(L, 1);
	const TimerContainer& timerContainer = timer->getTimerContainer();
	const float duration = static_cast<float>(luaL_checknumber(L, 2));
	const bool loop = lua_toboolean(L, 3) == 1;
	timer->setDuration(duration);
	timer->setBeginTime(timerContainer.getClock().getTime());
	timer->setLoop(loop);
	TimerContainer::callTimerUpdate(L, timer);
	return 0;
}

int l_Timer_stop(lua_State* L)
{
	Timer* timer = getTimer(L, 1);
	lua_pushboolean(L, timer->stop());
	return 1;
}

int l_Timer_getElapsedTime(lua_State* L)
{
	Timer* timer = getTimer(L, 1);
	lua_pushnumber(L, timer->getElapsedTime());
	return 1;
}

int l_Timer_onUpdate(lua_State* L)
{
	Timer* timer = getTimer(L, 1);
	flat::lua::SharedLuaReference<LUA_TFUNCTION> onUpdate;
	onUpdate.setIfNotNil(L, 2);
	timer->setOnUpdate(onUpdate);
	return 0;
}

int l_Timer_onEnd(lua_State* L)
{
	Timer* timer = getTimer(L, 1);	
	flat::lua::SharedLuaReference<LUA_TFUNCTION> onEnd;
	onEnd.setIfNotNil(L, 2);
	timer->setOnEnd(onEnd);
	return 0;
}

Timer* getTimer(lua_State* L, int index)
{
	return LuaTimer::get(L, index);
}

void pushTimer(lua_State* L, Timer* timer)
{
	if (timer != nullptr)
	{
		LuaTimer::pushNew(L, timer);
	}
	else
	{
		lua_pushnil(L);
	}
}

} // lua
} // timer
} // lua
} // flat


