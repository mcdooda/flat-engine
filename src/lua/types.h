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

int l_flat_type(lua_State* L);
int l_flat_typename(lua_State* L);

} // types
} // lua
} // flat

#endif // FLAT_LUA_TYPES_H


