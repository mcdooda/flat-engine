#ifndef FLAT_LUA_SHAREDLUAREFERENCE_H
#define FLAT_LUA_SHAREDLUAREFERENCE_H

#include <memory>
#include "luareference.h"

namespace flat
{
namespace lua
{

// SharedLuaReference is used to hold a lua object through c++

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
		
		inline void set(lua_State* L, int index)
		{
			FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
			LuaReference<LuaType>* reference = new LuaReference<LuaType>(L, index);
			Super::reset(reference);
		}
		
		inline void push(lua_State* L) const
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

		inline bool isEmpty() const
		{
			LuaReference<LuaType>* reference = Super::get();
			return reference == nullptr || reference->isEmpty();
		}
};

} // lua
} // flat

#endif // FLAT_LUA_SHAREDLUAREFERENCE_H



