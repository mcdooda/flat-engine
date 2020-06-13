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

	static const luaL_Reg Asset_lib_m[] = {
		{"getPath", l_Asset_getPath},
		{"getName", l_Asset_getName},
		{"getType", l_Asset_getType},

		{nullptr, nullptr}
	};
	lua.registerClass<LuaAsset>("flat.Asset", Asset_lib_m);

	static const luaL_Reg Asset_lib_s[] = {
		{"getDirectories",     l_Asset_getDirectories},
		{"getAssets",          l_Asset_getAssets},
		{"getParentDirectory", l_Asset_getParentDirectory},
		{"findFromName",       l_Asset_findFromName},

		{nullptr, nullptr}
	};
	luaL_newlib(L, Asset_lib_s);
	lua_setglobal(L, "Asset");

	return 0;
}

int l_Asset_getPath(lua_State* L)
{
	const Asset* asset = getAsset(L, 1);
	lua_pushstring(L, asset->getPath().string().c_str());
	return 1;
}

int l_Asset_getName(lua_State* L)
{
	const Asset* asset = getAsset(L, 1);
	lua_pushstring(L, asset->getName().c_str());
	return 1;
}

int l_Asset_getType(lua_State* L)
{
	const Asset* asset = getAsset(L, 1);
	lua_pushstring(L, asset->getType().c_str());
	return 1;
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

int l_Asset_getParentDirectory(lua_State* L)
{
	const std::string path = luaL_checkstring(L, 1);
	std::string parentDirectoryPath;
	const bool hasParent = flat::lua::getFlat(L).assetRepository->getParentDirectory(path, parentDirectoryPath);
	if (hasParent)
	{
		lua_pushstring(L, parentDirectoryPath.c_str());
	}
	else
	{
		lua_pushnil(L);
	}
	return 1;
}

int l_Asset_findFromName(lua_State* L)
{
	const std::string type = luaL_checkstring(L, 1);
	const std::string name = luaL_checkstring(L, 2);
	const Asset* asset = flat::lua::getFlat(L).assetRepository->findAssetFromName(type, name);
	pushAsset(L, asset);
	return 1;
}

const Asset* getAsset(lua_State* L, int index)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	return LuaAsset::get(L, index);
}

void pushAsset(lua_State* L, const Asset* asset)
{
	if (asset != nullptr)
	{
		LuaAsset::pushNew(L, asset);
	}
	else
	{
		lua_pushnil(L);
	}
}

} // flat::tool::lua::asset