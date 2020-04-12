#ifndef FLAT_TOOL_ASSET_LUA_ASSET_H
#define FLAT_TOOL_ASSET_LUA_ASSET_H

struct lua_State;

namespace flat
{
namespace tool { class Asset; }
namespace lua { class Lua; }
}

namespace flat::tool::lua::asset
{

int open(flat::lua::Lua& lua);

int l_Asset_getDirectories(lua_State* L);
int l_Asset_getAssets(lua_State* L);

void pushAsset(lua_State* L, const Asset* asset);

}

FLAT_LUA_PUSHABLE_TYPE(const flat::tool::Asset*, flat::tool::lua::asset::pushAsset(L, value))

#endif // FLAT_TOOL_ASSET_LUA_ASSET_H