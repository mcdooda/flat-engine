#ifndef FLAT_LUA_DEBUG_H
#define FLAT_LUA_DEBUG_H

#include "../debug/assert.h"

#ifdef FLAT_DEBUG

struct lua_State;

namespace flat
{
namespace lua
{
namespace debug
{

class ExpectStackGrowth
{
	public:
		ExpectStackGrowth(lua_State* L, int n, const char* func, const char* file, int line);
		~ExpectStackGrowth();

		static void setIgnoreAll();
		static void resetIgnoreAll();

	private:
		static bool ignoreAll;

		lua_State* m_L;
		int m_expectedTop;
		const char* m_func;
		const char* m_file;
		int m_line;
};

class IgnoreStackGrowth
{
	public:
		IgnoreStackGrowth(lua_State* L);
		~IgnoreStackGrowth();

	private:
		lua_State* m_L;
		int m_initialTop;
};

} // debug
} // lua
} // flat

#define FLAT_LUA_ASSERT_MSG(cond, L, format, ...) \
	if (!(cond)) \
	{ \
		fprintf(stderr, "assertion failed: " #cond " ; " format "\n" \
			"in file " __FILE__ ":%d\n" \
			"in function %s\n", \
			##__VA_ARGS__, __LINE__, FLAT_PRETTY_FUNCTION); \
		flat::lua::printStack(L); \
		FLAT_BREAK(); \
	}

#define FLAT_LUA_ASSERT(cond, L) \
	if (!(cond)) \
	{ \
		fprintf(stderr, "assertion failed: " #cond "\n" \
			"in file " __FILE__ ":%d\n" \
			"in function %s\n", \
			__LINE__, FLAT_PRETTY_FUNCTION); \
		flat::lua::printStack(L); \
		FLAT_BREAK(); \
	}

#define FLAT_LUA_EXPECT_STACK_GROWTH(L, n) \
	flat::lua::debug::ExpectStackGrowth expectStackGrowth(L, n, FLAT_PRETTY_FUNCTION, __FILE__, __LINE__);

#define FLAT_LUA_IGNORE_ALL_STACK_GROWTH() \
	flat::lua::debug::ExpectStackGrowth::setIgnoreAll();

#define FLAT_LUA_IGNORE_STACK_GROWTH(L) \
	flat::lua::debug::IgnoreStackGrowth ignoreStackGrowth(L);

#else // FLAT_DEBUG

#define FLAT_LUA_ASSERT_MSG(cond, L, format, ...) \
	{}

#define FLAT_LUA_ASSERT(cond, L) \
	{}

#define FLAT_LUA_EXPECT_STACK_GROWTH(L, n) \
	{}

#define FLAT_LUA_IGNORE_ALL_STACK_GROWTH() \
	{}

#endif // FLAT_DEBUG

#endif // FLAT_LUA_DEBUG_H



