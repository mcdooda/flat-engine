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
	using Super = std::shared_ptr<LuaReference<LuaType>>;
	public:
		SharedLuaReference() {}
		
		SharedLuaReference(lua_State* L, int index)
		{
			set(L, index);
		}
		
		inline void set(lua_State* L, int index)
		{
			FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
			LuaReference<LuaType>* currentReference = get();
			if (currentReference != nullptr)
			{
				currentReference->reset(L);
			}
			LuaReference<LuaType>* newReference = new LuaReference<LuaType>(L, index);
			Super::reset(newReference);
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
			LuaReference<LuaType>* reference = Super::get();
			FLAT_ASSERT(reference != nullptr);
			int luaReference = reference->getLuaReference();
			FLAT_ASSERT(luaReference != LUA_NOREF);
			lua_rawgeti(L, LUA_REGISTRYINDEX, luaReference);
		}

		inline bool isEmpty() const
		{
			LuaReference<LuaType>* reference = Super::get();
			return reference == nullptr || reference->isEmpty();
		}

		inline void reset(lua_State* L)
		{
			LuaReference<LuaType>* reference = Super::get();
			if (reference != nullptr)
			{
				reference->reset(L);
				Super::reset();
			}
		}
};

} // lua
} // flat

#endif // FLAT_LUA_SHAREDLUAREFERENCE_H



