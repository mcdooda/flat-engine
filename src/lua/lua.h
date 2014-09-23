#ifndef FLAT_LUA_LUA_H
#define FLAT_LUA_LUA_H

#include <lua5.2/lua.hpp>

namespace flat
{
namespace lua
{
lua_State* open();
void close(lua_State* L);

void doFile(lua_State* L, std::string fileName);

void printStack(lua_State* L);
const char* codeToString(int code);

int panic(lua_State* L);

} // lua
} // flat

#endif // FLAT_LUA_LUA_H



