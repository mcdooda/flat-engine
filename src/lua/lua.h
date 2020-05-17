#ifndef FLAT_LUA_LUA_H
#define FLAT_LUA_LUA_H

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <type_traits>
#include <filesystem>
#include <lua5.3/lua.hpp>

#include "lua/debug.h"
#include "lua/types.h"
#include "lua/timer/timercontainer.h"

#include "console/console.h"

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
		Lua(Flat& flat, const std::string& luaPath, const std::string& assetsPath);
		~Lua();

		void endFrame();

		void reset(Flat& flat);

		void doFile(const std::string& fileName);
		void loadFile(const std::string& fileName);
		void loadLib(const std::string& fileName, const std::string& globalName);

		void clearLoadedPackages();

		template <class T>
		void registerClass(const char* metatableName, const luaL_Reg* methods = nullptr);

		const char* getTypeName(size_t type) const;

		void setGarbageCollectorEnabled(bool enabled);
		void collectGarbage() const;

		template <class T, typename... Args>
		int protectedCall(T* object, void (T::*callbackMethod)(lua_State*, Args...), Args&&... args);

		template <class T, typename... Args>
		int protectedCall(const T* object, void (T::*callbackMethod)(lua_State*, Args...) const, Args&&... args);

		void pushVariable(std::initializer_list<const char*> variableNames) const;

		std::shared_ptr<timer::TimerContainer> newTimerContainer(const std::shared_ptr<time::Clock>& clock);

	private:
		void close(lua_State* L);

		static int l_flat_require(lua_State* L);
		void openRequire(lua_State* L);

		static int l_flat_dofile(lua_State* L);
		void openDofile(lua_State* L);

		static int l_flat_loadfile(lua_State* L);
		void openLoadfile(lua_State* L);

		static int l_flat_assetPath(lua_State* L);
		void openAssetPath(lua_State* L);

		void updateTimerContainers();

	public:
		lua_State* state;
		std::shared_ptr<timer::TimerContainer> defaultTimerContainer;

	private:
		std::string m_luaPath;
		std::string m_assetsPath;

		std::vector<std::weak_ptr<timer::TimerContainer>> m_timerContainers;

		std::unordered_map<size_t, std::string> m_typeHashToName;
};

void close(lua_State* L);

void doFile(lua_State* L, const std::filesystem::path& fileName);
void loadFile(lua_State* L, const std::filesystem::path& fileName);
void loadLib(lua_State* L, const std::filesystem::path& fileName, const std::string& globalName);

void clearLoadedPackages(lua_State* L);

lua_State* getMainThread(lua_State* L);

void printStack(lua_State* L);
void printValue(lua_State* L, int index, std::ostream& out = std::cout);
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
#ifdef FLAT_DEBUG
	auto pred = [metatableName](const std::unordered_map<size_t, std::string>::value_type& pair)
	{
		return pair.second == metatableName;
	};
	FLAT_ASSERT_MSG(
		std::find_if(m_typeHashToName.begin(), m_typeHashToName.end(), pred) == m_typeHashToName.end(),
		"Metatable '%s' is already registered", metatableName
	);
#endif
	std::string metatableNameStr = metatableName;
	size_t newTypeHash = std::hash<std::string>()(metatableNameStr);
	FLAT_ASSERT(m_typeHashToName.find(newTypeHash) == m_typeHashToName.end());
	m_typeHashToName[newTypeHash] = std::move(metatableNameStr);
	T::registerClass(state, newTypeHash, metatableName, methods);
	types::registerType(state, newTypeHash, metatableName);
}

template<class T, typename ...Args>
inline int Lua::protectedCall(T* object, void (T::*callbackMethod)(lua_State*, Args...), Args&&... args)
{
	using ProtectCallBlock = std::function<void(lua_State*)>;
	ProtectCallBlock protectedCallBlock = [object, callbackMethod, &args...](lua_State* L)
	{
		(object->*callbackMethod)(L, std::forward<Args>(args)...);
	};
	auto caller = [](lua_State* L)
	{
		ProtectCallBlock& protectedCallBlock = *static_cast<ProtectCallBlock*>(lua_touserdata(L, 1));
		protectedCallBlock(L);
		return 0;
	};
	lua_pushcfunction(state, caller);
	lua_pushlightuserdata(state, &protectedCallBlock);
	int code = lua_pcall(state, 1, 0, 0);
	if (code != LUA_OK)
	{
		const std::string errorMessage = luaL_checkstring(state, -1);
		lua_pop(state, 1);

		{
			FLAT_CONSOLE_COLOR(LIGHT_RED);
			std::cerr << errorMessage << std::endl;
		}
		printStack(state);
		std::cerr << std::endl;
	}
	return code;
}

template <class T, typename... Args>
inline int Lua::protectedCall(const T* object, void (T::*callbackMethod)(lua_State*, Args...) const, Args&&... args)
{
	using ProtectCallBlock = std::function<void(lua_State*)>;
	ProtectCallBlock protectedCallBlock = [object, callbackMethod, &args...](lua_State* L)
	{
		(object->*callbackMethod)(L, std::forward<Args>(args)...);
	};
	auto caller = [](lua_State* L)
	{
		ProtectCallBlock& protectedCallBlock = *static_cast<ProtectCallBlock*>(lua_touserdata(L, 1));
		protectedCallBlock(L);
		return 0;
	};
	lua_pushcfunction(state, caller);
	lua_pushlightuserdata(state, &protectedCallBlock);
	int code = lua_pcall(state, 1, 0, 0);
	if (code != LUA_OK)
	{
		const std::string errorMessage = luaL_checkstring(state, -1);
		lua_pop(state, 1);

		{
			FLAT_CONSOLE_COLOR(LIGHT_RED);
			std::cerr << errorMessage << std::endl;
		}
		printStack(state);
		std::cerr << std::endl;
	}
	return code;
}

} // lua
} // flat

#endif // FLAT_LUA_LUA_H



