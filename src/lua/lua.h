#ifndef FLAT_LUA_LUA_H
#define FLAT_LUA_LUA_H

#include <string>

struct lua_State;

namespace flat
{
class Flat;
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

class Lua
{
	public:
		Lua(Flat& flat);
		~Lua();

		void doFile(const std::string& fileName);
		void loadFile(const std::string& fileName);
		void loadLib(const std::string& fileName, const std::string& globalName);

		void clearLoadedPackages();

	public:
		lua_State* state;
};

void close(lua_State* L);

void doFile(lua_State* L, const std::string& fileName);
void loadFile(lua_State* L, const std::string& fileName);
void loadLib(lua_State* L, const std::string& fileName, const std::string& globalName);

void clearLoadedPackages(lua_State* L);

lua_State* getMainThread(lua_State* L);

void printStack(lua_State* L);
const char* codeToString(int code);

int panic(lua_State* L);

Game& getGame(lua_State* L);

} // lua
} // flat

#endif // FLAT_LUA_LUA_H



