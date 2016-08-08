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
time::Time* getTime(lua_State* L);

} // lua
} // flat

#ifdef FLAT_DEBUG

#define FLAT_LUA_ASSERT(cond, L) \
	if (!(cond)) { std::cerr << "assertion failed: " #cond "\n" << std::endl; flat::lua::printStack(L); FLAT_BREAK(); }

#define FLAT_LUA_ASSERT_MSG(cond, L, format, ...) \
	if (!(cond)) { fprintf(stderr, "assertion failed: " #cond " ; " format "\n", ##__VA_ARGS__); flat::lua::printStack(L); FLAT_BREAK(); }

#else // FLAT_DEBUG

#define FLAT_LUA_ASSERT(cond, L) \
	{}

#define FLAT_LUA_ASSERT_MSG(cond, L, format, ...) \
	{}

#endif // FLAT_DEBUG

#endif // FLAT_LUA_LUA_H



