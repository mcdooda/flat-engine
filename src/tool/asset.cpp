#include <filesystem>

#include "asset.h"

#include "lua/lua.h"

namespace flat::tool
{

static const std::filesystem::path assetMetaDataFileName = ".asset.lua";

Asset::Asset(const std::filesystem::path& path) :
	m_path(std::filesystem::relative(path))
{

}

void Asset::loadDetails(lua_State* L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	luaL_dofile(L, getMetaDataFilePath().string().c_str());
	luaL_argcheck(L, lua_istable(L, -1), -1, "Asset metadata file must return a table value");

	lua_getfield(L, -1, "type");
	m_type = luaL_checkstring(L, -1);

	lua_pop(L, 2);
}

std::filesystem::path Asset::getMetaDataFilePath() const
{
	return m_path / assetMetaDataFileName;
}

} // flat::tool