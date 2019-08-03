#include "lua/thread.h"
#include "lua/lua.h"

namespace flat
{
namespace lua
{

Thread::Thread() :
	m_status(LUA_OK)
{
	FLAT_DEBUG_ONLY(sanityCheck();)
}

void Thread::set(lua_State* L, int index)
{
	FLAT_DEBUG_ONLY(sanityCheck();)
	m_function.set(L, index);
	FLAT_DEBUG_ONLY(sanityCheck();)
}

void Thread::reset()
{
	m_function.reset();
	m_thread.reset();
	m_status = LUA_OK;
	FLAT_DEBUG_ONLY(sanityCheck();)
}

bool Thread::start(int numArgs)
{
	FLAT_DEBUG_ONLY(sanityCheck();)
	FLAT_ASSERT(isEmpty());

	lua_State* L = m_function.getLuaState();
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, -numArgs);

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
			stop();
		}
		else if (m_status != LUA_YIELD)
		{
			FLAT_LUA_IGNORE_ALL_STACK_GROWTH();
			lua_error(L1);
			return false;
		}

		lua_pop(L, numArgs + 1);
	}

	FLAT_DEBUG_ONLY(sanityCheck();)
	return true;
}

int Thread::update(int numResults)
{
	FLAT_DEBUG_ONLY(sanityCheck();)
	FLAT_ASSERT(isRunning());

	lua_State* L = m_function.getLuaState();
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, numResults);

		m_thread.push(L);
		lua_State* L1 = lua_tothread(L, -1);
		FLAT_ASSERT(L1 != nullptr);

		m_status = lua_resume(L1, nullptr, 0);

		if (numResults > 0)
		{
			lua_settop(L1, numResults);
		}

		if (m_status == LUA_OK)
		{
			stop();
		}
		else if (m_status != LUA_YIELD)
		{
			lua_error(L1);
		}

		lua_pop(L, 1);

		if (numResults > 0)
		{
			lua_xmove(L1, L, numResults);
		}

		FLAT_DEBUG_ONLY(sanityCheck();)
	}

	FLAT_DEBUG_ONLY(sanityCheck();)
	return m_status;
}

void Thread::stop()
{
	reset();
}

#ifdef FLAT_DEBUG
void Thread::sanityCheck()
{
	if (!m_function.isEmpty())
	{
		if (!m_thread.isEmpty())
		{
			FLAT_ASSERT(m_status == LUA_YIELD);
		}
		else
		{
			FLAT_ASSERT(m_status == LUA_OK);
		}
	}
	else
	{
		FLAT_ASSERT(m_thread.isEmpty());
		FLAT_ASSERT(m_status == LUA_OK);
	}
}
#endif // FLAT_DEBUG

} // lua
} // flat

