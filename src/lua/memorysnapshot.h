#ifndef FLAT_LUA_MEMORYSNAPSHOT_H
#define FLAT_LUA_MEMORYSNAPSHOT_H

#include <unordered_map>
#include <lua5.3/lua.hpp>

namespace flat
{
class Lua;
namespace lua
{
namespace snapshot
{

int open(lua_State* L);

int l_flat_lua_snapshot_diff(lua_State* L);

namespace capture
{

enum class MarkSourceType : std::uint8_t
{
	TABLEKEY,
	TABLEVALUE,
	UPVALUE,
	METATABLE,
	REGISTRY,
	USERVALUE
};

struct MarkSource
{
	MarkSourceType type;
	std::string description;
};

using MarkedMap = std::unordered_map<const void*, std::vector<MarkSource>>;

void markObject(lua_State* L, int index, MarkedMap& markedMap, MarkSource markSource);

void markPointer(lua_State* L, int index, MarkedMap& markedMap, MarkSource markSource);
bool isMarked(lua_State* L, int index, const MarkedMap& markedMap);

void markString(lua_State* L, int index, MarkedMap& markedMap, MarkSource markSource);
void markFunction(lua_State* L, int index, MarkedMap& markedMap, MarkSource markSource);
void markThread(lua_State* L, int index, MarkedMap& markedMap, MarkSource markSource);
void markTable(lua_State* L, int index, MarkedMap& markedMap, MarkSource markSource);
void markLightUserData(lua_State* L, int index, MarkedMap& markedMap, MarkSource markSource);
void markUserData(lua_State* L, int index, MarkedMap& markedMap, MarkSource markSource);

} // capture

} // snapshot
} // lua
} // flat

static int l_snapshot(lua_State *L);

#endif // FLAT_LUA_MEMORYSNAPSHOT_H


