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
		Lua(Flat& flat, const std::string& luaPath, const std::string& assetsPath);
		~Lua();

		void doFile(const std::string& fileName);
		void loadFile(const std::string& fileName);
		void loadLib(const std::string& fileName, const std::string& globalName);

		void clearLoadedPackages();

	private:
		static int l_flat_require(lua_State* L);
		void openRequire(lua_State* L);

		static int l_flat_dofile(lua_State* L);
		void openDofile(lua_State* L);

		static int l_flat_loadfile(lua_State* L);
		void openLoadfile(lua_State* L);

		static int l_flat_assetPath(lua_State* L);
		void openAssetPath(lua_State* L);

	public:
		lua_State* state;

	private:
		std::string m_luaPath;
		std::string m_assetsPath;
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



