#ifdef FLAT_DEBUG

#include <iostream>
#include <lua5.3/lua.hpp>

#include "lua/debug.h"
#include "lua/lua.h"

#include "debug/assert.h"

namespace flat
{
namespace lua
{
namespace debug
{

bool ExpectStackGrowth::ignoreAll;

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
	if (!ignoreAll)
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
	else
	{
		lua_settop(m_L, m_expectedTop);
	}
}

void ExpectStackGrowth::setIgnoreAll()
{
	ignoreAll = true;
}

void ExpectStackGrowth::resetIgnoreAll()
{
	ignoreAll = false;
}

IgnoreStackGrowth::IgnoreStackGrowth(lua_State* L) :
	m_L(L)
{
	ExpectStackGrowth::setIgnoreAll();
	m_initialTop = lua_gettop(L);
}

IgnoreStackGrowth::~IgnoreStackGrowth()
{
	lua_settop(m_L, m_initialTop);
	ExpectStackGrowth::resetIgnoreAll();
}

} // debug
} // lua
} // flat

#endif // FLAT_DEBUG



