#include "path.h"

#include "lua/lua.h"

#include "sharp/ai/navigation/path.h"

#include "misc/lua/vector2.h"

namespace flat::sharp::ai::navigation::lua
{

int open(flat::lua::Lua& lua)
{
	lua_State* L = lua.state;
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	static const luaL_Reg Path_lib_m[] = {
		{"getPoints",              l_Path_getPoints},
		{"getUniqueCellPositions", l_Path_getUniqueCellPositions},
		{"copy",                   l_Path_copy},
		{"insert",                 l_Path_insert},
		{"eachTile",               l_Path_eachCell},
		{nullptr, nullptr}
	};
	lua.registerClass<LuaPath>("CG.Path", Path_lib_m);

	// Path static methods
	static const luaL_Reg Path_lib_s[] = {

		{nullptr, nullptr}
	};

	luaL_newlib(L, Path_lib_s);
	lua_setglobal(L, "Path");

	return 0;
}

int l_Path_getPoints(lua_State* L)
{
	std::shared_ptr<Path> path = getPath(L, 1);
	flat::lua::table::pushVector(L, path->getPoints());
	return 1;
}

int l_Path_getUniqueCellPositions(lua_State* L)
{
	std::shared_ptr<Path> path = getPath(L, 1);
	flat::lua::table::pushVector(L, path->getUniqueCellPositions());
	return 1;
}

int l_Path_copy(lua_State* L)
{
	std::shared_ptr<Path> path = getPath(L, 1);
	const std::shared_ptr<Path> copied = std::make_shared<Path>(*path);
	pushPath(L, copied);
	return 1;
}

int l_Path_insert(lua_State* L)
{
	std::shared_ptr<Path> path = getPath(L, 1);
	std::shared_ptr<Path> other = getPath(L, 2);
	path->appendPath(*other);
	return 0;
}

int l_Path_eachCell(lua_State* L)
{
	std::shared_ptr<Path> path = getPath(L, 1);
	luaL_checktype(L, 2, LUA_TFUNCTION);
	for (const flat::Vector2& position : path->getUniqueCellPositions())
	{
		lua_pushvalue(L, 2);
		flat::lua::pushVector2(L, position);
		lua_call(L, 1, 0);
	}
	return 0;
}

void pushPath(lua_State* L, const std::shared_ptr<Path>& path)
{
	if (path != nullptr)
	{
		LuaPath::pushNew(L, path);
	}
	else
	{
		lua_pushnil(L);
	}
}

std::shared_ptr<Path> getPath(lua_State* L, int index)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	return LuaPath::get(L, index);
}

} // flat::sharp::ai::navigation::lua


