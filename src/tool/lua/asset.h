#ifndef FLAT_TOOL_ASSET_LUA_ASSET_H
#define FLAT_TOOL_ASSET_LUA_ASSET_H

struct lua_State;

namespace flat::tool::lua::asset
{

int open(lua_State* L);

int l_Asset_findAllAssetsOfType(lua_State* L);

}

#endif // FLAT_TOOL_ASSET_LUA_ASSET_H