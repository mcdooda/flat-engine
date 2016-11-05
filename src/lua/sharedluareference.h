#ifndef FLAT_LUA_SHAREDLUAREFERENCE_H
#define FLAT_LUA_SHAREDLUAREFERENCE_H

#include <memory>
#include "../debug/assert.h"

namespace flat
{
namespace lua
{

// SharedLuaReference is used to hold a lua object through c++

template <int LuaType>
class LuaReference
{
	public:
		LuaReference(lua_State* L, int index) :
			m_luaState(L),
			m_luaReference(LUA_NOREF)
		{
			FLAT_ASSERT(lua_type(L, index) == LuaType);
			lua_pushvalue(L, index);
			m_luaReference = luaL_ref(L, LUA_REGISTRYINDEX);
		}
		
		~LuaReference()
		{
			luaL_unref(m_luaState, LUA_REGISTRYINDEX, m_luaReference);
			m_luaReference = LUA_NOREF;
		}
		
		inline lua_State* getLuaState() const { return m_luaState; }
		inline int getLuaReference() const { return m_luaReference; }
		
	private:
		lua_State* m_luaState;
		int m_luaReference;
};

template <int LuaType>
class SharedLuaReference : public std::shared_ptr<LuaReference<LuaType>>
{
	typedef std::shared_ptr<LuaReference<LuaType>> Super;
	public:
		SharedLuaReference() {}
		
		SharedLuaReference(lua_State* L, int index)
		{
			set(L, index);
		}
		
		void set(lua_State* L, int index)
		{
			FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
			LuaReference<LuaType>* reference = new LuaReference<LuaType>(L, index);
			Super::reset(reference);
		}
		
		void push(lua_State* L) const
		{
			FLAT_LUA_EXPECT_STACK_GROWTH(L, 1);
			LuaReference<LuaType>* reference = Super::get();
			FLAT_ASSERT(reference != nullptr);
			int luaReference = reference->getLuaReference();
			FLAT_ASSERT(luaReference != LUA_NOREF);
			lua_rawgeti(L, LUA_REGISTRYINDEX, luaReference);
		}
		
		inline lua_State* getLuaState() const
		{
			LuaReference<LuaType>* reference = Super::get();
			FLAT_ASSERT(reference != nullptr);
			return reference->getLuaState();
		}
};

} // lua
} // flat

#endif // FLAT_LUA_SHAREDLUAREFERENCE_H



