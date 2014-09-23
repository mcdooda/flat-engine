#include <iostream>
#include <cstdlib>
#include "lua.h"

namespace flat
{
namespace lua
{

lua_State* open()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	
	// remove os lib
	lua_pushnil(L);
	lua_setglobal(L, "os");
	
	// panic function
	lua_atpanic(L, panic);
	
	return L;
}

void close(lua_State* L)
{
	lua_close(L);
}

void doFile(lua_State* L, std::string fileName)
{
	luaL_loadfile(L, fileName.c_str());
	lua_call(L, 0, 0);
}

void loadLib(lua_State* L, std::string fileName, std::string globalName)
{
	luaL_loadfile(L, fileName.c_str());
	lua_call(L, 0, 1);
	lua_setglobal(L, globalName.c_str());
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
	exit(1);
	return 0;
}

} // lua
} // flat



