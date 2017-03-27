#ifdef FLAT_DEBUG

#include <iostream>
#include <lua5.3/lua.hpp>
#include "debug.h"
#include "lua.h"
#include "../debug/assert.h"

namespace flat
{
namespace lua
{
namespace debug
{

ExpectStackGrowth::ExpectStackGrowth(lua_State* L, int n, const char* func, const char* file, int line) :
	m_L(L),
	m_func(func),
	m_file(file),
	m_line(line)
{
	m_expectedTop = lua_gettop(L) + n;
}

ExpectStackGrowth::~ExpectStackGrowth()
{
	int top = lua_gettop(m_L);
	if (top != m_expectedTop)
	{
		std::cerr << "lua_gettop(L) = " << top << ", expected " << m_expectedTop << std::endl
			<< "in file " << m_file << ":" << m_line << std::endl
			<< "in function " << m_func << std::endl;
		flat::lua::printStack(m_L);
		FLAT_BREAK();
	}
}

} // debug
} // lua
} // flat

#endif // FLAT_DEBUG



