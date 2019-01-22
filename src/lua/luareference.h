#ifndef FLAT_LUA_LUAREFERENCE_H
#define FLAT_LUA_LUAREFERENCE_H

#include <lua5.3/lua.hpp>
#include "debug.h"
#include "table.h"
#include "../util/variadichelpers.h"
#include "../debug/helpers.h"
#include "../profiler/profilersection.h"

namespace flat
{
namespace lua
{

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

		inline void push(lua_State* L) const
		{
			FLAT_LUA_EXPECT_STACK_GROWTH(L, 1);
			FLAT_ASSERT(m_luaReference != LUA_NOREF);
			lua_rawgeti(L, LUA_REGISTRYINDEX, m_luaReference);
			FLAT_DEBUG_ONLY(luaL_checktype(L, -1, LuaType);)
		}
		
		inline lua_State* getLuaState() const { return m_luaState; }
		inline int getLuaReference() const { return m_luaReference; }
		inline bool isEmpty() const { return m_luaReference == LUA_NOREF; }

		inline operator bool() const
		{
			return m_luaReference != LUA_NOREF;
		}

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

		template<typename PushArgumentsCallback, typename = std::enable_if_t<LuaType == LUA_TFUNCTION>>
		void callFunction(PushArgumentsCallback pushArgumentsCallback) const
		{
			FLAT_PROFILE("Call lua function (no result)");

			FLAT_ASSERT(m_luaReference != LUA_NOREF);
			FLAT_ASSERT(m_luaState != nullptr);
			lua_State* L = m_luaState;
			{
				FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

#ifdef FLAT_DEBUG
				lua_getglobal(L, "debug");
				lua_getfield(L, -1, "traceback");
				lua_remove(L, -2);
#endif
				const int top = lua_gettop(L);

				push(L);
				pushArgumentsCallback(L);

				const int numArguments = lua_gettop(L) - top - 1;

#ifdef FLAT_DEBUG
				if (lua_pcall(L, numArguments, 0, top) != LUA_OK)
				{
					std::cerr << "Lua function call error: " << luaL_checkstring(L, -1) << std::endl;
					lua_pop(L, 1);
				}
				lua_pop(L, 1); // pop traceback function
#else
				lua_call(L, numArguments, 0);
#endif
			}
		}

		template<typename... Args>
		void call(Args&&... args) const
		{
			callFunction(
				[&args...](lua_State* L)
				{
					FLAT_VARIADIC_EXPAND(lua::pushValue(L, args));
				}
			);
		}

		template<typename PushArgumentsCallback, typename HandleResultsCallback, typename = std::enable_if_t<LuaType == LUA_TFUNCTION>>
		void callFunction(PushArgumentsCallback pushArgumentsCallback, int numResults, HandleResultsCallback handleResultsCallback) const
		{
			FLAT_PROFILE("Call lua function");

			FLAT_ASSERT(m_luaReference != LUA_NOREF);
			FLAT_ASSERT(m_luaState != nullptr);
			lua_State* L = m_luaState;
			{
				FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

#ifdef FLAT_DEBUG
				lua_getglobal(L, "debug");
				lua_getfield(L, -1, "traceback");
				lua_remove(L, -2);
#endif
				const int top = lua_gettop(L);

				push(L);
				pushArgumentsCallback(L);
				const int numArguments = lua_gettop(L) - top - 1;

#ifdef FLAT_DEBUG
				if (lua_pcall(L, numArguments, numResults, top) == LUA_OK)
				{
					lua_remove(L, top); // pop traceback function
#else
				lua_call(L, numArguments, numResults);
#endif
				{
					FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
					handleResultsCallback(L);
				}
				lua_pop(L, numResults);

#ifdef FLAT_DEBUG
				}
				else
				{
					std::cerr << "Lua function call error: " << luaL_checkstring(L, -1) << std::endl;
					lua_pop(L, 2); // pop error message and traceback function
				}
#endif
			}
		}
		
	protected:
		lua_State* m_luaState;
		int m_luaReference;
};

} // lua
} // flat

#endif // FLAT_LUA_LUAREFERENCE_H



