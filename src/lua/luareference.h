#ifndef FLAT_LUA_LUAREFERENCE_H
#define FLAT_LUA_LUAREFERENCE_H

#include <lua5.3/lua.hpp>
#include "debug.h"
#include "../debug/helpers.h"

namespace flat
{
namespace lua
{

FLAT_OPTIMIZE_OFF()

template <int LuaType>
class LuaReference
{
	public:
		LuaReference() :
			m_luaState(nullptr),
			m_luaReference(LUA_NOREF)
		{

		}

		LuaReference(lua_State* L, int index)
		{
			set(L, index);
		}
		
		~LuaReference()
		{
			reset();
		}

		inline void set(lua_State* L, int index)
		{
			FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
			if (lua_type(L, index) != LuaType)
			{
				std::string error = std::string("Wrong type for LuaReference: ") + lua_typename(L, LuaType) + " expected, got " + luaL_typename(L, index);
				luaL_argerror(L, index, error.c_str());
			}
			lua_pushvalue(L, index);
			m_luaReference = luaL_ref(L, LUA_REGISTRYINDEX);
			
			m_luaState = getMainThread(L);
		}

		inline void setIfNotNil(lua_State* L, int index)
		{
			if (!lua_isnil(L, index))
			{
				set(L, index);
			}
		}
		
		inline lua_State* getLuaState() const { return m_luaState; }
		inline int getLuaReference() const { return m_luaReference; }
		inline bool isEmpty() const { return m_luaReference == LUA_NOREF; }

	protected:
		inline void reset()
		{
			if (m_luaState != nullptr)
			{
				FLAT_LUA_EXPECT_STACK_GROWTH(m_luaState, 0);
				luaL_unref(m_luaState, LUA_REGISTRYINDEX, m_luaReference);
				m_luaState = nullptr;
				m_luaReference = LUA_NOREF;
			}
#ifdef FLAT_DEBUG
			else
			{
				FLAT_ASSERT(m_luaReference == LUA_NOREF);
			}
#endif
		}
		
	protected:
		lua_State* m_luaState;
		int m_luaReference;
};

FLAT_OPTIMIZE_ON()

} // lua
} // flat

#endif // FLAT_LUA_LUAREFERENCE_H



