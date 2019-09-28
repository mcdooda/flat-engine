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

int Thread::resume(int numArgs, int numResults)
{
	FLAT_DEBUG_ONLY(sanityCheck();)

	lua_State* L = m_function.getLuaState();
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, numResults - numArgs);

		const int firstParamIndex = lua_absindex(L, -numArgs);

		lua_State* L1 = nullptr;
		if (!isRunning())
		{
			// create new thread
			L1 = lua_newthread(L);
			m_thread.set(L, -1);

			lua_pop(L, 1);

			m_function.push(L1);
		}
		else
		{
			// push existing thread
			m_thread.push(L);
			L1 = lua_tothread(L, -1);

			lua_pop(L, 1);
		}

		FLAT_ASSERT(L1 != nullptr);

		lua_xmove(L, L1, numArgs);

		m_status = lua_resume(L1, L, numArgs);

		if (m_status == LUA_OK || m_status == LUA_YIELD)
		{
			if (numResults > 0)
			{
				lua_settop(L1, numResults);
				lua_xmove(L1, L, numResults);
			}

			if (m_status == LUA_OK)
			{
				stop();
			}
		}
		else
		{
			printStack(L1);
			FLAT_LUA_IGNORE_ALL_STACK_GROWTH();
			stop();
		}
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

