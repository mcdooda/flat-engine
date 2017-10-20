#ifndef FLAT_LUA_MEMORYSNAPSHOT_H
#define FLAT_LUA_MEMORYSNAPSHOT_H

#include <lua5.3/lua.hpp>

namespace flat
{
class Lua;
namespace lua
{
namespace snapshot
{

int open(lua_State* L);

int l_flat_lua_snapshot_diff(lua_State* L);

} // snapshot
} // lua
} // flat

static int l_snapshot(lua_State *L);

#endif // FLAT_LUA_MEMORYSNAPSHOT_H


