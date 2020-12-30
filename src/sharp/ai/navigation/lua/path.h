#ifndef FLAT_SHARP_AI_NAVIGATION_LUA_PATH_H
#define FLAT_SHARP_AI_NAVIGATION_LUA_PATH_H

#include <memory>

#include "lua/sharedcppreference.h"

struct lua_State;

namespace flat::lua
{
class Lua;
}

namespace flat::sharp::ai::navigation
{
class Path;
}

namespace flat::sharp::ai::navigation::lua
{
using LuaPath = flat::lua::SharedCppValue<std::shared_ptr<Path>>;

int open(flat::lua::Lua& lua);
int l_Path_getPoints(lua_State* L);
int l_Path_getUniqueCellPositions(lua_State* L);
int l_Path_copy(lua_State* L);
int l_Path_insert(lua_State* L);
int l_Path_eachCell(lua_State* L);

std::shared_ptr<Path> getPath(lua_State* L, int index);
void pushPath(lua_State* L, const std::shared_ptr<Path>& path);

} // flat::sharp::ai::navigation::lua

#endif // FLAT_SHARP_AI_NAVIGATION_LUA_PATH_H
