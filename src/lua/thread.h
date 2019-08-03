#ifndef FLAT_LUA_THREAD_H
#define FLAT_LUA_THREAD_H

#include "lua/uniqueluareference.h"

namespace flat
{
namespace lua
{

class Thread
{
	public:
		Thread();

		void set(lua_State* L, int index);
		void reset();

		int resume(int numArgs, int numResults);

		void stop();

		inline bool isRunning() const { return m_status == LUA_YIELD; }
		inline bool isFinished() const { return m_status == LUA_OK; }
		inline bool isEmpty() const { return m_thread.isEmpty(); }
		inline bool hasFunction() const { return !m_function.isEmpty(); }

		FLAT_DEBUG_ONLY(const flat::lua::UniqueLuaReference<LUA_TTHREAD>& getThread() const { return m_thread; })

	protected:
		FLAT_DEBUG_ONLY(void sanityCheck();)

	protected:

		flat::lua::UniqueLuaReference<LUA_TFUNCTION> m_function;
		flat::lua::UniqueLuaReference<LUA_TTHREAD> m_thread;
		int m_status;
};

} // lua
} // flat

#endif // FLAT_LUA_THREAD_H


