#ifndef FLAT_LUA_TYPES_H
#define FLAT_LUA_TYPES_H

#include <lua5.3/lua.hpp>

namespace flat
{
namespace lua
{
namespace types
{

int open(lua_State* L);

void registerNativeType(lua_State* L, int type, const char* typeName);
void registerType(lua_State* L, size_t newTypeHash, const char* typeName);

int l_flat_type(lua_State* L);
int l_flat_typetostring(lua_State* L);

} // types
} // lua
} // flat

#endif // FLAT_LUA_TYPES_H


