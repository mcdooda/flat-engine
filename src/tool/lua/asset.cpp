#include <lua5.3/lua.hpp>

#include "flat.h"
#include "asset.h"
#include "tool/asset.h"
#include "tool/assetrepository.h"
#include "lua/lua.h"

namespace flat::tool::lua::asset
{

using LuaAsset = flat::lua::SharedCppValue<const Asset*>;

int open(flat::lua::Lua& lua)
{
	lua_State* L = lua.state;
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	lua.registerClass<LuaAsset>("flat.Asset");

	static const luaL_Reg Asset_lib_s[] = {
		{"getDirectories", l_Asset_getDirectories},
		{"getAssets",      l_Asset_getAssets},

		{nullptr, nullptr}
	};
	luaL_newlib(L, Asset_lib_s);
	lua_setglobal(L, "Asset");

	return 0;
}

int l_Asset_getDirectories(lua_State* L)
{
	const std::string path = luaL_checkstring(L, 1);
	std::vector<std::string> directories = flat::lua::getFlat(L).assetRepository->getDirectories(path);
	flat::lua::table::pushVector(L, directories);
	return 1;
}

int l_Asset_getAssets(lua_State* L)
{
	const std::string path = luaL_checkstring(L, 1);
	std::vector<const Asset*> assets = flat::lua::getFlat(L).assetRepository->getAssets(path);
	flat::lua::table::pushVector(L, assets);
	return 1;
}

void pushAsset(lua_State* L, const Asset* asset)
{
	LuaAsset::pushNew(L, asset);
}

} // flat::tool::lua::asset