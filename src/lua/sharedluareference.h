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
			LuaReference<LuaType>* reference = new LuaReference<LuaType>(L, index);
			Super::reset(reference);
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
			reference->push(L);
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

		template<typename PushArgumentsCallback, typename = std::enable_if_t<LuaType == LUA_TFUNCTION>>
		void callFunction(PushArgumentsCallback pushArgumentsCallback) const
		{
			const LuaReference<LuaType>* reference = Super::get();
			FLAT_ASSERT(reference != nullptr);
			reference->callFunction(pushArgumentsCallback);
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
			const LuaReference<LuaType>* reference = Super::get();
			FLAT_ASSERT(reference != nullptr);
			reference->callFunction(pushArgumentsCallback, numResults, handleResultsCallback);
		}
};

} // lua
} // flat

#endif // FLAT_LUA_SHAREDLUAREFERENCE_H



