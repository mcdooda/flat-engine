#ifndef FLAT_LUA_LUAREFERENCE_H
#define FLAT_LUA_LUAREFERENCE_H

#include <mutex>
#include <shared_mutex>
#include <lua5.3/lua.hpp>

#include "debug.h"

#include "../debug/helpers.h"

namespace flat
{
namespace lua
{

template <int LuaType>
class SharedLuaReference;

static std::shared_mutex referenceMutex;

template <int LuaType>
class LuaReference
{
	friend class SharedLuaReference<LuaType>;

	public:
		LuaReference() :
			m_luaReference(LUA_NOREF)
		{

		}

		LuaReference(lua_State* L, int index)
		{
			set(L, index);
		}
		
		~LuaReference()
		{
			FLAT_ASSERT_MSG(m_luaReference == LUA_NOREF, "The reference must be unref'ed before");
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

			{
				std::unique_lock<std::shared_mutex> lock(referenceMutex);
				m_luaReference = luaL_ref(L, LUA_REGISTRYINDEX);
			}
		}

		inline void setIfNotNil(lua_State* L, int index)
		{
			if (!lua_isnil(L, index))
			{
				set(L, index);
			}
		}
		
		inline int getLuaReference() const { return m_luaReference; }
		inline bool isEmpty() const { return m_luaReference == LUA_NOREF; }

	protected:
		inline void reset(lua_State* L)
		{
			if (m_luaReference != LUA_NOREF)
			{
				FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
				{
					std::unique_lock<std::shared_mutex> lock(referenceMutex);
					luaL_unref(L, LUA_REGISTRYINDEX, m_luaReference);
				}
				m_luaReference = LUA_NOREF;
			}
		}
		
	protected:
		int m_luaReference;
};

} // lua
} // flat

#endif // FLAT_LUA_LUAREFERENCE_H



