#include "thread.h"
#include "lua.h"

namespace flat
{
namespace lua
{

Thread::Thread() :
	m_status(LUA_OK)
{

}

void Thread::set(lua_State* L, int index)
{
	m_function.set(L, index);
}

bool Thread::start(lua_State* L, int numArgs)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, -numArgs);
	FLAT_ASSERT(isEmpty());

	lua_State* L1 = lua_newthread(L);
	m_thread.set(L, -1);

	m_function.push(L);
		
	int firstParamIndex = lua_absindex(L, -2 - numArgs);
	for (int i = 0; i < numArgs; ++i)
	{
		int index = firstParamIndex + i; 
		lua_pushvalue(L, index);
	}
	lua_xmove(L, L1, numArgs + 1);

	m_status = lua_resume(L1, nullptr, numArgs);
	if (m_status == LUA_OK)
	{
		m_thread.reset(L);
	}
	else if (m_status != LUA_YIELD)
	{
		FLAT_LUA_IGNORE_ALL_STACK_GROWTH();
		lua_error(L1);
		return false;
	}

	lua_pop(L, numArgs + 1);
	return true;
}

void Thread::update(lua_State* L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	FLAT_ASSERT(isRunning());

	m_thread.push(L);
	lua_State* L1 = lua_tothread(L, -1);
	FLAT_ASSERT(L1 != nullptr);

	m_status = lua_resume(L1, nullptr, 0);
	if (m_status == LUA_OK)
	{
		stop(L);
	}
	else if (m_status != LUA_YIELD)
	{
		lua_error(L1);
	}

	lua_pop(L, 1);
}

void Thread::stop(lua_State* L)
{
	m_thread.reset(L);
	m_status = LUA_OK;
}

void Thread::reset(lua_State* L)
{
	m_function.reset(L);
	m_thread.reset(L);
}

} // lua
} // flat

