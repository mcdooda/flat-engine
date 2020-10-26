#include <cstdlib>
#include <lua5.3/lua.hpp>

#include "lua/lua.h"
#include "lua/memorysnapshot.h"
#include "lua/types.h"
#include "lua/timer/lua/timer.h"

#include "flat.h"
#include "flat/game.h"
#include "time/lua/time.h"
#include "input/lua/keyboard.h"
#include "input/lua/mouse.h"
#include "input/lua/gamepads.h"
#include "video/lua/image.h"
#include "audio/lua/audio.h"
#include "sharp/ui/lua/ui.h"
#include "misc/lua/vector2.h"
#include "misc/lua/vector3.h"
#include "file/lua/file.h"
#include "profiler/lua/profiler.h"

#include "tool/lua/asset.h"

namespace flat
{
namespace lua
{
static char gameRegistryIndex = 'F';

Lua::Lua(Flat& flat, const std::string& luaPath, const std::string& assetsPath) :
	state(nullptr)
{
	m_luaPath = luaPath;
	if (m_luaPath[m_luaPath.size() - 1] != '/')
		m_luaPath += '/';

	m_assetsPath = assetsPath;
	if (m_assetsPath[m_assetsPath.size() - 1] != '/')
		m_assetsPath += '/';

	reset(flat);
}

Lua::~Lua()
{
	close(state);
}

void Lua::endFrame()
{
	updateTimerContainers();

#ifdef FLAT_DEBUG
	if (lua_gettop(state) != 0)
	{
		flat::lua::printStack(state);
		FLAT_ASSERT_MSG(false, "Please keep the stack clean!");
	}
#endif
}

void Lua::reset(Flat& flat)
{
	if (state != nullptr)
	{
		close(state);
	}

	m_typeHashToName.clear();

	state = luaL_newstate();

	lua_State* L = state;
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
		luaL_openlibs(L);

		// panic function
		lua_atpanic(L, panic);

		// store Game in the registry
		lua_pushlightuserdata(L, &flat);
		lua_rawsetp(L, LUA_REGISTRYINDEX, &gameRegistryIndex);

		// flat. libraries
		lua_newtable(L);
#ifdef FLAT_DEBUG
		lua_newtable(L);
		lua_pushcfunction(L, [](lua_State* L) { FLAT_BREAK(); return 0; });
		lua_setfield(L, -2, "debugbreak");
		lua_pushcfunction(L, [](lua_State* L) { printStack(L); return 0; });
		lua_setfield(L, -2, "printstack");
#else
		lua_pushboolean(L, false);
#endif
		lua_setfield(L, -2, "debug"); // flat.debug = {}/false

		lua_newtable(L);
		lua_setfield(L, -2, "lua"); // flat.lua = {}

		lua_setglobal(L, "flat");

		types::open(L);

		timer::lua::open(*this);
		time::lua::open(L);
		input::lua::keyboard::open(L);
		input::lua::mouse::open(L);
		input::lua::gamepads::open(L);
		video::lua::image::open(L);
		audio::lua::open(*this);
		sharp::ui::lua::open(flat, *this);

		snapshot::open(*this);
		profiler::lua::open(L);

		lua::openVector2(*this);
		lua::openVector3(*this);

		file::lua::open(*this);

		openRequire(L);
		openDofile(L);
		openLoadfile(L);
		openAssetPath(L);

		tool::lua::asset::open(*this);

		// dofile flat-engine/lua/init.lua
		lua_pushstring(L, m_luaPath.c_str());
		lua_pushcclosure(L, l_flat_dofile, 1);
		lua_pushstring(L, "init.lua");
		lua_call(L, 1, 0);
	}

	defaultTimerContainer = newTimerContainer(flat.time->defaultClock);
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

void Lua::clearLoadedPackages()
{
	lua::clearLoadedPackages(state);
}

const char* Lua::getTypeName(size_t type) const
{
	std::unordered_map<size_t, std::string>::const_iterator it = m_typeHashToName.find(type);
	if (it != m_typeHashToName.cend())
	{
		return it->second.c_str();
	}
	return nullptr;
}

void Lua::setGarbageCollectorEnabled(bool enabled)
{
	if (enabled)
	{
		lua_gc(state, LUA_GCRESTART, 0);
	}
	else
	{
		lua_gc(state, LUA_GCSTOP, 0);
	}
}

void Lua::collectGarbage() const
{
	lua_gc(state, LUA_GCCOLLECT, 0);
}

void Lua::pushVariable(std::initializer_list<const char*> variableNames) const
{
	lua_State* L = state;
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 1);
		lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
		int top = lua_gettop(L);
		for (const char* variableName : variableNames)
		{
			lua_pushstring(L, variableName);
			lua_rawget(L, -2);
		}
		lua_replace(L, top);
		lua_settop(L, top);
	}
}

std::shared_ptr<timer::TimerContainer> Lua::newTimerContainer(const std::shared_ptr<time::Clock>& clock)
{
	std::shared_ptr<timer::TimerContainer> timerContainer = std::make_shared<timer::TimerContainer>(clock);
	m_timerContainers.emplace_back(timerContainer);
	return timerContainer;
}

void Lua::close(lua_State* L)
{
	defaultTimerContainer.reset();

	sharp::ui::lua::close(L);

	lua_pushnil(L);
	lua_rawsetp(L, LUA_REGISTRYINDEX, &gameRegistryIndex);

	lua_close(L);
}

int Lua::l_flat_require(lua_State* L)
{
	const char* path = luaL_checkstring(L, 1);
	lua_settop(L, 1);
	lua_pushvalue(L, lua_upvalueindex(1)); // push the flat lua/ directory path
	lua_pushvalue(L, 1);
	lua_concat(L, 2);
	int top = lua_gettop(L);
	lua_getglobal(L, "require");
	lua_pushvalue(L, -2);
	lua_call(L, 1, LUA_MULTRET);
	return lua_gettop(L) - top;
}

void Lua::openRequire(lua_State* L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	lua_getglobal(L, "flat");
	lua_pushstring(L, m_luaPath.c_str());
	lua_pushcclosure(L, l_flat_require, 1);
	lua_setfield(L, -2, "require");
	lua_pop(L, 1);
}

int Lua::l_flat_dofile(lua_State* L)
{
	const char* flatLuaPath = luaL_checkstring(L, lua_upvalueindex(1)); // push the flat lua/ directory path
	const char* path = luaL_checkstring(L, 1);
	int top = lua_gettop(L);
	if (luaL_dofile(L, (std::string(flatLuaPath) + path).c_str()))
	{
		lua_error(L);
	}
	return lua_gettop(L) - top;
}


void Lua::openDofile(lua_State* L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	lua_getglobal(L, "flat");
	lua_pushstring(L, m_luaPath.c_str());
	lua_pushcclosure(L, l_flat_dofile, 1);
	lua_setfield(L, -2, "dofile");
	lua_pop(L, 1);
}

int Lua::l_flat_loadfile(lua_State * L)
{
	const char* flatLuaPath = luaL_checkstring(L, lua_upvalueindex(1)); // push the flat lua/ directory path
	const char* path = luaL_checkstring(L, 1);
	if (luaL_loadfile(L, (std::string(flatLuaPath) + path).c_str()))
	{
		lua_error(L);
	}

	// set environment if given
	if (!lua_isnoneornil(L, 2))
	{
		luaL_checktype(L, 2, LUA_TTABLE);
		lua_pushvalue(L, 2);
		lua_setupvalue(L, -2, 1);
	}

	return 1;
}

void Lua::openLoadfile(lua_State * L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	lua_getglobal(L, "flat");
	lua_pushstring(L, m_luaPath.c_str());
	lua_pushcclosure(L, l_flat_loadfile, 1);
	lua_setfield(L, -2, "loadfile");
	lua_pop(L, 1);
}

int Lua::l_flat_assetPath(lua_State * L)
{
	lua_pushvalue(L, lua_upvalueindex(1)); // push the flat assets/ directory path
	luaL_checkstring(L, 1);
	lua_pushvalue(L, 1);
	lua_concat(L, 2);
	return 1;
}

void Lua::openAssetPath(lua_State * L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	lua_getglobal(L, "flat");
	lua_pushstring(L, m_assetsPath.c_str());
	lua_pushcclosure(L, l_flat_assetPath, 1);
	lua_setfield(L, -2, "assetPath");
	lua_pop(L, 1);
}

void Lua::updateTimerContainers()
{
	for (std::vector<std::weak_ptr<timer::TimerContainer>>::iterator it = m_timerContainers.begin(); it != m_timerContainers.end(); )
	{
		std::weak_ptr<timer::TimerContainer>& timerContainerWeakPtr = *it;
		if (timerContainerWeakPtr.expired())
		{
			it = m_timerContainers.erase(it);
		}
		else
		{
			timer::TimerContainer* timerContainer = timerContainerWeakPtr.lock().get();
			timerContainer->updateTimers(state);
			++it;
		}
	}
}

void doFile(lua_State* L, const std::filesystem::path& fileName)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	luaL_dofile(L, fileName.string().c_str());
}

void loadFile(lua_State* L, const std::filesystem::path& fileName)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 1);
	int code = luaL_loadfile(L, fileName.string().c_str());
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

void loadLib(lua_State* L, const std::filesystem::path& fileName, const std::string& globalName)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	luaL_loadfile(L, fileName.string().c_str());
	lua_call(L, 0, 1);
	lua_setglobal(L, globalName.c_str());
}

void clearLoadedPackages(lua_State* L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	lua_getglobal(L, "package");
	lua_getfield(L, -1, "loaded");
	flat::lua::table::clear(L, -1);
	lua_pop(L, 2);
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
	{
		FLAT_CONSOLE_COLOR(RED);
		std::cout << "--- Lua Debug (" << L << ") ==========" << std::endl;
	}
	lua_Debug debug;
	std::cout << "  CALL STACK" << std::endl;
	bool callStackIsEmpty = true;
	for (int level = 0; lua_getstack(L, level, &debug); ++level)
	{
		callStackIsEmpty = false;
		lua_getinfo(L, "Snl", &debug);
		std::cout << "    " << debug.short_src << ':' << debug.currentline;
		if (debug.name != nullptr)
		{
			std::cout << " in ";
			if (debug.namewhat != nullptr)
			{
				std::cout << debug.namewhat << " ";
			}
			std::cout << debug.name;
		}
		std::cout << std::endl;
	}
	if (callStackIsEmpty)
	{
		std::cout << "    <callstack is empty>" << std::endl;
	}
	
	int top = lua_gettop(L);
	std::cout << "  LUA STACK" << std::endl;
	
	for (int i = top; i >= 1; --i)
	{
		std::cout << "    #" << i << "/-" << top - i + 1 << "\t : ";
		printValue(L, i);
		std::cout << std::endl;
	}
	if (top == 0)
	{
		std::cout << "    <lua stack is empty>" << std::endl;
	}
	else if (top < 0)
	{
		std::cout << "    <lua stack is broken: top = " << top << ">" << std::endl;
	}

	{
		FLAT_CONSOLE_COLOR(RED);
		std::cout << "--- Lua Debug ======" << std::endl;
	}
}

void printValue(lua_State* L, int index, std::ostream& out)
{
	const char* str = nullptr;
	switch (lua_type(L, index))
	{
		case LUA_TLIGHTUSERDATA:
			out << "lightuserdata : " << lua_touserdata(L, index);
			break;

		case LUA_TUSERDATA:
			str = luaL_tolstring(L, index, nullptr);
			out << "userdata ____ : " << lua_touserdata(L, index) << ": " << str;
			lua_pop(L, 1);
			break;

		case LUA_TTABLE:
			out << "table _______ : " << lua_topointer(L, index) << (lua_topointer(L, index) == lua_topointer(L, LUA_REGISTRYINDEX) ? " (REGISTRY)" : "");
			break;

		case LUA_TNIL:
			out << "nil";
			break;

		case LUA_TFUNCTION:
			out << "function ____ : " << lua_topointer(L, index);
			break;

		case LUA_TNUMBER:
			out << "number ______ : " << lua_tonumber(L, index);
			break;

		case LUA_TBOOLEAN:
			out << "boolean _____ : " << (lua_toboolean(L, index) ? "true" : "false");
			break;

		case LUA_TSTRING:
			out << "string ______ : " << lua_tostring(L, index);
			break;

		case LUA_TTHREAD:
			out << "thread ______ : " << lua_topointer(L, index);
			break;

		case LUA_TNONE:
			out << "none ________ : " << lua_topointer(L, index);
			break;

		default:
			out << "unknown type : " << lua_topointer(L, index) << " (" << lua_typename(L, index) << " ?)";
			break;
	}
}

const char* codeToString(int code)
{
	static const char* ok = "LUA_OK";
	static const char* yield = "LUA_YIELD";
	static const char* errRun = "LUA_ERRRUN";
	static const char* errSyntax = "LUA_ERRSYNTAX";
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

		case LUA_ERRSYNTAX:
			return errSyntax;
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
	lua_getglobal(L, "debug");
	lua_getfield(L, -1, "debug");
	lua_call(L, 0, 0);
	exit(1);
	return 0;
}

Flat& getFlat(lua_State* L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	lua_rawgetp(L, LUA_REGISTRYINDEX, &gameRegistryIndex);
	Flat* flat = static_cast<Flat*>(lua_touserdata(L, -1));
	FLAT_ASSERT(flat != nullptr);
	lua_pop(L, 1);
	return *flat;
}

} // lua
} // flat



