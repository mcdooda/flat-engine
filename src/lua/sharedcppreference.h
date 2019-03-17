#ifndef FLAT_LUA_SHAREDCPPREFERENCE_H
#define FLAT_LUA_SHAREDCPPREFERENCE_H

#include <memory>
#include <string>
#include <lua5.3/lua.hpp>

#include "lua/debug.h"

#include "flat.h"
#include "memory/memory.h"

namespace flat
{
namespace lua
{

template <class T>
class SharedCppValue
{
	public:
		using ValueType = T;
		static std::string metatableName;

		static T& get(lua_State* L, int index)
		{
			FLAT_ASSERT_MSG(!metatableName.empty(), "Type not registered in ClassRegistry");
			void* userData = luaL_checkudata(L, index, metatableName.c_str());
			FLAT_ASSERT(userData != nullptr);
			return *static_cast<ValueType*>(userData);
		}

		template <typename... ConstructorArgs>
		static ValueType& pushNew(lua_State* L, ConstructorArgs... constructorArgs)
		{
			FLAT_ASSERT_MSG(!metatableName.empty(), "Type not registered in ClassRegistry");
			FLAT_LUA_EXPECT_STACK_GROWTH(L, 1);
			ValueType* userData = static_cast<ValueType*>(lua_newuserdata(L, sizeof(ValueType)));
			new (userData) ValueType(constructorArgs...);
			luaL_getmetatable(L, metatableName.c_str());
			lua_setmetatable(L, -2);
			return *userData;
		}

		static int l_destroyObject(lua_State* L)
		{
			ValueType* value = &get(L, 1);
			value->~ValueType();
			FLAT_DEBUG_ONLY(memset(value, FLAT_WIPE_VALUE, sizeof(ValueType)));
			return 0;
		}

		static void registerClass(lua_State* L, size_t newTypeHash, const char* metatableName, const luaL_Reg* methods)
		{
			FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

			SharedCppValue<T>::metatableName = metatableName;

			// create object metatable
			luaL_newmetatable(L, metatableName);

			// mt.__index = mt
			lua_pushvalue(L, -1);
			lua_setfield(L, -2, "__index");

			// __gc
			lua_pushcfunction(L, &SharedCppValue<T>::l_destroyObject);
			lua_setfield(L, -2, "__gc");

			// type for flat.type
			lua_pushinteger(L, newTypeHash);
			lua_setfield(L, -2, "type");

			// others methods
			if (methods != nullptr)
			{
				luaL_setfuncs(L, methods, 0);
			}

			lua_pop(L, 1);
		}
};

template <class T>
std::string SharedCppValue<T>::metatableName;

template <class T>
class SharedCppReference : public SharedCppValue<std::shared_ptr<T>>
{
	using Super = SharedCppValue<std::shared_ptr<T>>;
	public:
		static T& get(lua_State* L, int index)
		{
			ValueType& sharedPointer = Super::get(L, index);
			FLAT_ASSERT(sharedPointer != nullptr);
			return *sharedPointer.get();
		}
};

} // lua
} // flat

#endif // FLAT_LUA_SHAREDCPPREFERENCE_H



