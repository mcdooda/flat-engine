#ifndef FLAT_LUA_DEBUG_H
#define FLAT_LUA_DEBUG_H

#ifdef FLAT_DEBUG

#include <lua5.2/lua.hpp>

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

	private:
		lua_State* m_L;
		int m_expectedTop;
		const char* m_func;
		const char* m_file;
		int m_line;
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
			##__VA_ARGS__, __LINE__, __PRETTY_FUNCTION__); \
		flat::lua::printStack(L); \
		FLAT_BREAK(); \
	}

#define FLAT_LUA_ASSERT(cond, L) \
	if (!(cond)) \
	{ \
		std::cerr << "assertion failed: " #cond << std::endl \
			"in file " __FILE__ ":" << __LINE__ << std::endl \
			"in function " << __PRETTY_FUNCTION__ << std::endl; \
		flat::lua::printStack(L); \
		FLAT_BREAK(); \
	}

#define FLAT_LUA_EXPECT_STACK_GROWTH(L, n) \
	flat::lua::debug::ExpectStackGrowth expectStackGrowth(L, n, __PRETTY_FUNCTION__, __FILE__, __LINE__);

#else // FLAT_DEBUG

#define FLAT_LUA_ASSERT_MSG(cond, L, format, ...) \
	{}

#define FLAT_LUA_ASSERT(cond, L) \
	{}

#define FLAT_LUA_EXPECT_STACK_GROWTH(L, n) \
	{}

#endif // FLAT_DEBUG

#endif // FLAT_LUA_DEBUG_H



