#ifndef FLAT_LUA_LUA_H
#define FLAT_LUA_LUA_H

#include <string>
#include <lua5.2/lua.hpp>

namespace flat
{
class Game;
namespace time
{
class Time;
}
namespace input
{
class Input;
}
namespace lua
{
lua_State* open(Game& game);
void close(lua_State* L);

void doFile(lua_State* L, const std::string& fileName);
void loadLib(lua_State* L, const std::string& fileName, const std::string& globalName);

lua_State* getMainThread(lua_State* L);

void printStack(lua_State* L);
const char* codeToString(int code);

int panic(lua_State* L);

Game& getGame(lua_State* L);
time::Time* getTime(lua_State* L);
input::Input* getInput(lua_State* L);

template <class T>
T& getGameAs(lua_State* L)
{
	Game& game = getGame(L);
	FLAT_ASSERT(dynamic_cast<T*>(&game) != nullptr);
	return static_cast<T&>(game);
}

} // lua
} // flat

#endif // FLAT_LUA_LUA_H



