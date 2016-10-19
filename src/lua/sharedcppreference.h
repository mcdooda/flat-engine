#ifndef FLAT_LUA_SHAREDCPPREFERENCE_H
#define FLAT_LUA_SHAREDCPPREFERENCE_H

#include <memory>
#include <string>
#include <lua5.2/lua.hpp>
#include "debug.h"

namespace flat
{
namespace lua
{

template <class T>
class SharedCppReference
{
	public:
		typedef std::shared_ptr<T> PointerType;
		static std::string mtName;

		static T& get(lua_State* L, int index)
		{
			return *getSharedPointer(L, index).get();
		}

		static const PointerType& getSharedPointer(lua_State* L, int index)
		{
			FLAT_ASSERT_MSG(!mtName.empty(), "Type not registered in ClassRegistry");
			void* userData = luaL_checkudata(L, index, mtName.c_str());
			FLAT_ASSERT(userData != nullptr);
			return *static_cast<PointerType*>(userData);
		}

		template <typename... ConstructorArgs>
		static PointerType& pushNew(lua_State* L, ConstructorArgs... constructorArgs)
		{
			FLAT_ASSERT_MSG(!mtName.empty(), "Type not registered in ClassRegistry");
			FLAT_LUA_EXPECT_STACK_GROWTH(L, 1);
			PointerType* userData = static_cast<PointerType*>(lua_newuserdata(L, sizeof(PointerType)));
			new (userData) PointerType(constructorArgs...);
			luaL_getmetatable(L, mtName.c_str());
			lua_setmetatable(L, -2);
			return *userData;
		}

		static int l_destroyObject(lua_State* L)
		{
			PointerType* sharedPointer = static_cast<PointerType*>(luaL_checkudata(L, 1, mtName.c_str()));
			sharedPointer->~PointerType();
			return 0;
		}
};

template <class T>
std::string SharedCppReference<T>::mtName;

class ClassRegistry
{
	public:
		template <class T>
		static void registerClass(const char* mtName, lua_State* L, const luaL_Reg* methods = nullptr)
		{
			FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

			SharedCppReference<T>::mtName = mtName;

			// create object metatable
			luaL_newmetatable(L, SharedCppReference<T>::mtName.c_str());

			// mt.__index = mt
			lua_pushvalue(L, -1);
			lua_setfield(L, -2, "__index");

			// __gc
			lua_pushcfunction(L, &SharedCppReference<T>::l_destroyObject);
			lua_setfield(L, -2, "__gc");

			// others methods
			if (methods != nullptr)
			{
				luaL_setfuncs(L, methods, 0);
			}

			lua_pop(L, 1);
		}
};

} // lua
} // flat

#endif // FLAT_LUA_SHAREDCPPREFERENCE_H



