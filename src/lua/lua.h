#ifndef FLAT_LUA_LUA_H
#define FLAT_LUA_LUA_H

#include <string>
#include <vector>
#include <lua5.3/lua.hpp>
#include "types.h"

namespace flat
{
class Flat;
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
		static constexpr int FIRST_CLASS_TYPE_INDEX = LUA_NUMTAGS;

	public:
		Lua(Flat& flat, const std::string& luaPath, const std::string& assetsPath);
		~Lua();

		void doFile(const std::string& fileName);
		void loadFile(const std::string& fileName);
		void loadLib(const std::string& fileName, const std::string& globalName);

		void clearLoadedPackages();

		template <class T>
		void registerClass(const char* metatableName, const luaL_Reg* methods = nullptr);

		const char* getTypeName(int type) const;

		void collectGarbage() const;

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

		int m_nextTypeIndex;
		std::vector<std::string> m_typeIndexToName;
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

Flat& getFlat(lua_State* L);

template <class T>
T& getFlatAs(lua_State* L)
{
	Flat* flat = &getFlat(L);
	FLAT_ASSERT(dynamic_cast<T*>(flat) != nullptr);
	return static_cast<T&>(*flat);
}

template<class T>
inline void Lua::registerClass(const char* metatableName, const luaL_Reg* methods)
{
	int newTypeIndex = m_nextTypeIndex++;
	m_typeIndexToName.push_back(metatableName);
	T::registerClass(state, newTypeIndex, metatableName, methods);
	types::registerType(state, newTypeIndex, metatableName);
}

} // lua
} // flat

#endif // FLAT_LUA_LUA_H



