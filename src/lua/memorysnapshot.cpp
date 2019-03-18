#include <iomanip>
#include <iostream>
#include <fstream>

#include "lua/memorysnapshot.h"
#include "lua/lua.h"
#include "lua/sharedcppreference.h"
#include "lua/debug.h"

namespace flat
{
namespace lua
{
namespace snapshot
{

using LuaMemorySnapshot = flat::lua::SharedCppReference<MemorySnapshot>;

int open(Lua& lua)
{
	lua_State* L = lua.state;
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	static const luaL_Reg flat_lua_snapshot_lib_s[] = {
		{"snapshot",   l_flat_lua_snapshot_snapshot},
		{"diff",       l_flat_lua_snapshot_diff},
		{"references", l_flat_lua_snapshot_references},

		{"monitor",    l_flat_lua_monitor},

		{nullptr, nullptr}
	};

	lua_getglobal(L, "flat");
	lua_getfield(L, -1, "lua");
	luaL_newlib(L, flat_lua_snapshot_lib_s);
	lua_setfield(L, -2, "snapshot");

	lua.registerClass<LuaMemorySnapshot>("flat.MemorySnapshot");

	lua_pop(L, 2);

	return 0;
}

int l_flat_lua_snapshot_snapshot(lua_State* L)
{
	std::shared_ptr<MemorySnapshot> snapshot = std::make_shared<MemorySnapshot>(L);
	LuaMemorySnapshot::pushNew(L, snapshot);
	return 1;
}

int l_flat_lua_snapshot_diff(lua_State* L)
{
	MemorySnapshot& snapshot1 = LuaMemorySnapshot::get(L, 1);
	MemorySnapshot& snapshot2 = LuaMemorySnapshot::get(L, 2);
	const char* diffFile = luaL_checkstring(L, 3);
	MemorySnapshot diff(snapshot1, snapshot2);
	diff.writeToFile(diffFile);
	diff.writeDigestToFile(diffFile);
	return 0;
}


int l_flat_lua_snapshot_references(lua_State* L)
{
	const void* object = lua_topointer(L, 1);
	const char* referencesFile = luaL_checkstring(L, 2);
	MemorySnapshot snapshot(L);
	MemorySnapshot references(snapshot, object);
	references.writeToFile(referencesFile);
	references.writeDigestToFile(referencesFile);
	return 0;
}

static int l_newindex(lua_State* L)
{
	std::cerr << "New index: [" << lua_tostring(L, 2) << "] = " << lua_tostring(L, 3) << std::endl;
	printStack(L);
	FLAT_BREAK();

	// call the original __index metamethod
	lua_pushvalue(L, lua_upvalueindex(1));
	switch (lua_type(L, -1))
	{
	case LUA_TTABLE:
		lua_pushvalue(L, 2);
		lua_pushvalue(L, 3);
		lua_rawset(L, -3);
		break;

	case LUA_TFUNCTION:
		lua_pushvalue(L, 1);
		lua_pushvalue(L, 2);
		lua_pushvalue(L, 3);
		lua_call(L, 3, 0);
		break;
	}
	return 0;
}

int l_flat_lua_monitor(lua_State* L)
{
	luaL_argcheck(L, lua_type(L, 1) == LUA_TTABLE || lua_type(L, 1) == LUA_TUSERDATA, 1, "Cannot monitor values that are not a table or a userdata");
	if (lua_getmetatable(L, 1))
	{
		lua_getfield(L, -1, "__newindex");
		lua_pushcclosure(L, l_newindex, 1);
		lua_setfield(L, -2, "__newindex");
	}
	else
	{
		lua_newtable(L);
		lua_pushnil(L);
		lua_pushcclosure(L, l_newindex, 1);
		lua_setfield(L, -2, "__newindex");
		lua_setmetatable(L, 1);
	}
	return 0;
}

MemorySnapshot::MemorySnapshot(lua_State* L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	lua_pushvalue(L, LUA_REGISTRYINDEX);
	MarkSource registryMarkSource("[registry]", MarkSourceType::REGISTRY);
	markObject(L, -1, registryMarkSource);
	lua_pop(L, 1);
}

MemorySnapshot::MemorySnapshot(const MemorySnapshot& first, const MemorySnapshot& second)
{
	for (const std::pair<const void*, ObjectDescription>& markedObject : second.m_markedMap)
	{
		MarkedMap::const_iterator it = first.m_markedMap.find(markedObject.first);
		if (it == first.m_markedMap.end())
		{
			m_markedMap[markedObject.first] = markedObject.second;
		}
	}
}


MemorySnapshot::MemorySnapshot(const MemorySnapshot& snapshot, const void* object)
{
	MarkedMap::const_iterator it = snapshot.m_markedMap.find(object);

	if (it != snapshot.m_markedMap.end())
	{
		m_markedMap.insert(*it);
	}
}

void MemorySnapshot::writeToFile(const std::string& fileName) const
{
	if (m_markedMap.empty())
		return;

	std::fstream f(fileName + ".lualeak", std::ios_base::out);

	for (const std::pair<const void*, ObjectDescription>& markedObject : m_markedMap)
	{
		f << markedObject.second.value << " (" << markedObject.second.definition << "):\n";
		for (const MarkSource& markSource : markedObject.second.sources)
		{
			f << "\t- " << markSource.description << '\n';
		}
		f << "\n";
	}
}

void MemorySnapshot::writeDigestToFile(const std::string& fileName) const
{
	if (m_markedMap.empty())
		return;

	std::unordered_map<std::string, std::pair<const void*, int>> digest;

	for (const std::pair<const void*, ObjectDescription>& markedObject : m_markedMap)
	{
		const std::string& definition = markedObject.second.definition;
		auto it = digest.find(definition);
		if (it == digest.end())
		{
			digest[definition] = std::make_pair(markedObject.first, 1);
		}
		else
		{
			it->second.second++;
		}
	}

	std::vector<std::pair<std::string, std::pair<const void*, int>>> sortedDigest(digest.begin(), digest.end());
	std::sort(
		sortedDigest.begin(),
		sortedDigest.end(),
		[](const std::pair<std::string, std::pair<const void*, int>>& a, const std::pair<std::string, std::pair<const void*, int>>& b)
		{
			return b.second.second < a.second.second;
		}
	);

	std::fstream f(fileName + ".lualeak.digest", std::ios_base::out);

	for (const std::pair<std::string, std::pair<const void*, int>>& digestLine : sortedDigest)
	{
		f << std::setw(4) << digestLine.second.second << ": " << digestLine.first << '\n';
	}
}

bool MemorySnapshot::isMarked(lua_State* L, int index) const
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	const void* pointer = lua_topointer(L, index);
	return m_markedMap.find(pointer) != m_markedMap.end();
}

bool MemorySnapshot::markPointer(lua_State* L, int index, MarkSource markSource)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	index = lua_absindex(L, index);
	const void* pointer = lua_topointer(L, index);
	markSource.object = pointer;
	markSource.objectType = lua_type(L, index);
	MarkedMap::iterator it = m_markedMap.find(pointer);
	if (it != m_markedMap.end())
	{
		it->second.sources.push_back(markSource);
		return true;
	}
	else
	{
		ObjectDescription& description = m_markedMap[pointer];
		description.value = luaL_tolstring(L, index, nullptr);
		lua_pop(L, 1);
		std::string typeName;
		if (luaL_getmetafield(L, index, "__name") == LUA_TSTRING)
		{
			typeName = lua_tostring(L, -1);
			description.value = typeName + ": " + description.value;
			lua_pop(L, 1);
		}
		else
		{
			int type = lua_type(L, index);
			typeName = lua_typename(L, type);
		}
		description.sources.push_back(markSource);
		description.definition = std::string("unknown definition for ") + typeName;
		return false;
	}
}

void MemorySnapshot::markObject(lua_State* L, int index, MarkSource markSource)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	switch (lua_type(L, index))
	{
	case LUA_TFUNCTION:
		markFunction(L, index, markSource);
		break;
	case LUA_TTHREAD:
		markThread(L, index, markSource);
		break;
	case LUA_TTABLE:
		markTable(L, index, markSource);
		break;
	case LUA_TLIGHTUSERDATA:
		markLightUserData(L, index, markSource);
		break;
	case LUA_TUSERDATA:
		markUserData(L, index, markSource);
		break;
	}
}

void MemorySnapshot::markFunction(lua_State* L, int index, MarkSource markSource)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	if (!markPointer(L, index, markSource))
	{
		lua_pushvalue(L, index);
		lua_Debug ar;
		lua_getinfo(L, ">nS", &ar);
		ObjectDescription& description = m_markedMap[lua_topointer(L, index)];
		description.definition = std::string(ar.namewhat) + " " + (ar.name ? ar.name : "<anonymous function>") + " from " + ar.short_src + ":" + std::to_string(ar.linedefined);

		{
			FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
			int numUpValues = 0;
			while (lua_getupvalue(L, index, numUpValues + 1))
			{
				FLAT_LUA_EXPECT_STACK_GROWTH(L, -1);
				++numUpValues;

				markObject(L, -1, markSource);
				lua_pop(L, 1);
			}
		}
	}
}

void MemorySnapshot::markThread(lua_State* L, int index, MarkSource markSource)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	if (!markPointer(L, index, markSource))
	{
		lua_pushvalue(L, index);
		lua_Debug ar;
		lua_getinfo(L, ">nS", &ar);
		ObjectDescription& description = m_markedMap[lua_topointer(L, index)];
		description.definition = std::string(ar.namewhat) + " " + (ar.name ? ar.name : "<anonymous thread>") + " from " + ar.short_src + ":" + std::to_string(ar.linedefined);

		lua_State* L2 = lua_tothread(L, index);
		int top = lua_gettop(L2);
		for (int i = 1; i <= top; ++i)
		{
			markObject(L2, i, markSource);
		}

		// TODO: mark locals
	}
}

void MemorySnapshot::markTable(lua_State* L, int index, MarkSource markSource)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	if (!markPointer(L, index, markSource))
	{
		lua_pushvalue(L, index);
		lua_Debug ar;
		lua_getinfo(L, ">nS", &ar);
		ObjectDescription& description = m_markedMap[lua_topointer(L, index)];
		description.definition = std::string(ar.namewhat) + " " + (ar.name ? ar.name : "<anonymous table>") + " from " + ar.short_src + ":" + std::to_string(ar.linedefined);

		bool weakKeys = false;
		bool weakValues = false;
		{
			FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
			if (lua_getmetatable(L, index))
			{
				MarkSource metatableMarkSource(markSource, "[metatable]", MarkSourceType::METATABLE);
				markTable(L, -1, metatableMarkSource);

				hasWeakKeysAndValues(L, -1, weakKeys, weakValues);

				lua_pop(L, 1);
			}
		}

		{
			FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
			const int tableIndex = lua_absindex(L, index);
			lua_pushnil(L);
			while (lua_next(L, tableIndex) != 0)
			{
				FLAT_LUA_EXPECT_STACK_GROWTH(L, -1);

				if (!weakKeys)
				{
					FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
					MarkSource tableKeyMarkSource(markSource, "[key]", MarkSourceType::TABLEKEY);
					markObject(L, -2, tableKeyMarkSource);
				}

				if (!weakValues)
				{
					FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
					size_t length = 0;
					std::string key = luaL_tolstring(L, -2, nullptr);
					lua_pop(L, 1);
					MarkSource tableValueMarkSource(markSource, key, MarkSourceType::TABLEVALUE);
					markObject(L, -1, tableValueMarkSource);
				}

				lua_pop(L, 1);
			}
		}
	}
}

void MemorySnapshot::markLightUserData(lua_State* L, int index, MarkSource markSource)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	markPointer(L, index, markSource);
}

void MemorySnapshot::markUserData(lua_State* L, int index, MarkSource markSource)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	if (!markPointer(L, index, markSource))
	{
		if (lua_getmetatable(L, index))
		{
			lua_getfield(L, -1, "__name");
			std::string description = "[userdata metatable";
			if (!lua_isnil(L, -1))
			{
				description += " ";
				description += lua_tostring(L, -1);
			}
			description += "]";
			lua_pop(L, 1);
			MarkSource metatableMarkSource(markSource, description, MarkSourceType::METATABLE);
			markTable(L, -1, metatableMarkSource);
			lua_pop(L, 1);
		}

		lua_getuservalue(L, index);
		if (!lua_isnil(L, -1))
		{
			MarkSource uservalueMarkSource(markSource, "[uservalue]", MarkSourceType::USERVALUE);
			markObject(L, -1, uservalueMarkSource);
		}
		lua_pop(L, 1);
	}
}

void MemorySnapshot::hasWeakKeysAndValues(lua_State* L, int index, bool& weakKeys, bool& weakValues) const
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	int metatableIndex = lua_absindex(L, index);
	lua_pushliteral(L, "__mode");
	lua_rawget(L, metatableIndex);
	weakKeys = false;
	weakValues = false;
	if (lua_isstring(L, -1))
	{
		const char *mode = lua_tostring(L, -1);
		if (strchr(mode, 'k'))
		{
			weakKeys = true;
		}
		if (strchr(mode, 'v'))
		{
			weakValues = true;
		}
	}
	lua_pop(L, 1);
}

} // snapshot
} // lua
} // flat