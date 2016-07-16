#ifndef FLAT_LUA_LUA_H
#define FLAT_LUA_LUA_H

#include <string>
#include <lua5.2/lua.hpp>

namespace flat
{
class Game;
namespace lua
{
lua_State* open(Game* game);
void close(lua_State* L);

void doFile(lua_State* L, const std::string& fileName);
void loadLib(lua_State* L, const std::string& fileName, const std::string& globalName);

void printStack(lua_State* L);
const char* codeToString(int code);

int panic(lua_State* L);

Game* getGame(lua_State* L);

} // lua
} // flat

#endif // FLAT_LUA_LUA_H



