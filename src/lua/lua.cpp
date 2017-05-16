#include <iostream>
#include <cstdlib>
#include <lua5.3/lua.hpp>
#include "../flat.h"
#include "../flat/game.h"
#include "lua.h"
#include "../time/lua/time.h"
#include "../input/lua/mouse.h"
#include "../video/lua/image.h"
#include "../sharp/ui/lua/ui.h"

namespace flat
{
namespace lua
{
static char gameRegistryIndex = 'F';

Lua::Lua(Flat& flat)
{
	state = luaL_newstate();

	lua_State* L = state;
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
		luaL_openlibs(L);

		// remove os lib
		lua_pushnil(L);
		lua_setglobal(L, "os");

		// panic function
		lua_atpanic(L, panic);

		// store Game in the registry
		lua_pushlightuserdata(L, &flat);
		lua_rawsetp(L, LUA_REGISTRYINDEX, &gameRegistryIndex);

		time::lua::open(L);
		input::lua::mouse::open(L);
		video::lua::image::open(L);
		sharp::ui::lua::open(L);
	}
}

Lua::~Lua()
{
	lua_State* L = state;
	{
		sharp::ui::lua::close(L);

		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
		lua_pushnil(L);
		lua_rawsetp(L, LUA_REGISTRYINDEX, &gameRegistryIndex);
	}

	lua_close(L);
}

void Lua::doFile(const std::string& fileName)
{
	lua::doFile(state, fileName);
}

void Lua::loadFile(const std::string& fileName)
{
	lua::loadFile(state, fileName);
}

void Lua::loadLib(const std::string& fileName, const std::string& globalName)
{
	lua::loadLib(state, fileName, globalName);
}

void doFile(lua_State* L, const std::string& fileName)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	luaL_loadfile(L, fileName.c_str());
	lua_call(L, 0, 0);
}

void loadFile(lua_State* L, const std::string& fileName)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 1);
	int code = luaL_loadfile(L, fileName.c_str());
	if (code == LUA_ERRFILE)
	{
		lua_pop(L, 1);
		lua_pushnil(L);
	}
	else if (code != LUA_OK)
	{
		lua_error(L);
	}
}

void loadLib(lua_State* L, const std::string& fileName, const std::string& globalName)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	luaL_loadfile(L, fileName.c_str());
	lua_call(L, 0, 1);
	lua_setglobal(L, globalName.c_str());
}

lua_State* getMainThread(lua_State* L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_MAINTHREAD);
	luaL_checktype(L, -1, LUA_TTHREAD);
	lua_State* mainThread = lua_tothread(L, -1);
	lua_pop(L, 1);
	return mainThread;
}

void printStack(lua_State* L)
{
	std::cout << "--- Lua Debug (" << L << ") ==========" << std::endl;
	
	lua_Debug debug;
	std::cout << "\tCALL STACK" << std::endl;
	for (int level = 0; lua_getstack(L, level, &debug); level++)
	{
		lua_getinfo(L, "Snl", &debug);
		std::cout << "\t\tlevel #" << level << std::endl
		          << "\t\t\tname __________ : " << (debug.name ? debug.name : "") << std::endl
		          << "\t\t\tnamewhat ______ : " << (debug.namewhat ? debug.namewhat : "") << std::endl
		          << "\t\t\tsource ________ : " << debug.source << std::endl
		          << "\t\t\tshort_src _____ : " << debug.short_src << std::endl
		          << "\t\t\tlinedefined ___ : " << debug.linedefined << std::endl
		          << "\t\t\tlastlinedefined : " << debug.lastlinedefined << std::endl
		          << "\t\t\tcurrentline ___ : " << debug.currentline << std::endl << std::endl;
	}
	
	int top = lua_gettop(L);
	std::cout << "\tLUA STACK" << std::endl;
	
	for (int i = 1; i <= top; i++)
	{
		std::cout << "\t\t#" << i << " : ";
		switch(lua_type(L, i))
		{
			case LUA_TLIGHTUSERDATA:
			std::cout << "lightuserdata : " << lua_touserdata(L, i);
			break;
			
			case LUA_TUSERDATA:
			std::cout << "userdata ____ : " << lua_touserdata(L, i);
			break;
			
			case LUA_TTABLE:
			std::cout << "table _______ : " << lua_topointer(L, i) << (lua_topointer(L, i) == lua_topointer(L, LUA_REGISTRYINDEX)? " (REGISTRY)" : "");
			break;
			
			case LUA_TNIL:
			std::cout << "nil";
			break;
			
			case LUA_TFUNCTION:
			std::cout << "function ____ : " << lua_topointer(L, i);
			break;
			
			case LUA_TNUMBER:
			std::cout << "number ______ : " << lua_tonumber(L, i);
			break;
			
			case LUA_TBOOLEAN:
			std::cout << "boolean _____ : " << (lua_toboolean(L, i) ? "true" : "false");
			break;
			
			case LUA_TSTRING:
			std::cout << "string ______ : " << lua_tostring(L, i);
			break;
			
			case LUA_TTHREAD:
			std::cout << "thread ______ : " << lua_topointer(L, i);
			break;
			
			case LUA_TNONE:
			std::cout << "none ________ : " << lua_topointer(L, i);
			break;
			
			default:
			std::cout << "unknown type : " << lua_topointer(L, i) << " (" << lua_typename(L, i) << " ?)";
			break;
		}
		std::cout << std::endl;
	}
	std::cout << "--- Lua Debug ======" << std::endl;
}

const char* codeToString(int code)
{
	static const char* ok = "LUA_OK";
	static const char* yield = "LUA_YIELD";
	static const char* errRun = "LUA_ERRRUN";
	static const char* errMem = "LUA_ERRMEM";
	static const char* errErr = "LUA_ERRERR";
	static const char* errGcMm = "LUA_ERRGCMM";
	static const char* unknown = "(UNKNOWN)";
	
	switch (code)
	{
		case LUA_OK:
			return ok;
			break;
			
		case LUA_YIELD:
			return yield;
			break;
			
		case LUA_ERRRUN:
			return errRun;
			break;
			
		case LUA_ERRMEM:
			return errMem;
			break;
			
		case LUA_ERRERR:
			return errErr;
			break;
			
		case LUA_ERRGCMM:
			return errGcMm;
			break;
			
		default:
			return unknown;
	}
}

int panic(lua_State* L)
{
	std::cout << "~~~~ PANIC! ~~~~" << std::endl;
	printStack(L);
	std::cout << "~~~~ PANIC! ~~~~" << std::endl;
	FLAT_BREAK();
	exit(1);
	return 0;
}

Game& getGame(lua_State* L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	lua_rawgetp(L, LUA_REGISTRYINDEX, &gameRegistryIndex);
	Game* game = static_cast<Game*>(lua_touserdata(L, -1));
	FLAT_ASSERT(game != nullptr);
	lua_pop(L, 1);
	return *game;
}

} // lua
} // flat



