#ifndef FLAT_LUA_MEMORYSNAPSHOT_H
#define FLAT_LUA_MEMORYSNAPSHOT_H

#include <string>
#include <unordered_map>
#include <lua5.3/lua.hpp>

namespace flat
{
class Lua;
namespace lua
{
namespace snapshot
{

int open(Lua& lua);

int l_flat_lua_snapshot_snapshot(lua_State* L);
int l_flat_lua_snapshot_diff(lua_State* L);
int l_flat_lua_monitor(lua_State* L);

class MemorySnapshot
{
	public:
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
			const void* object;
			const void* parentObject;
			std::string description;
			int objectType;
			MarkSourceType sourceType;

			MarkSource(const std::string& description, MarkSourceType sourceType) :
				object(nullptr),
				parentObject(nullptr),
				description(description),
				sourceType(sourceType)
			{}

			MarkSource(const MarkSource& parentSource, const std::string& description, MarkSourceType sourceType) :
				parentObject(parentSource.object),
				description(parentSource.description + " > " + description),
				sourceType(sourceType)
			{}
		};

		struct ObjectDescription
		{
			std::string value;
			std::string definition;
			std::vector<MarkSource> sources;
		};

		using MarkedMap = std::unordered_map<const void*, ObjectDescription>;

	public:
		MemorySnapshot() = delete;
		MemorySnapshot(const MemorySnapshot&) = delete;
		MemorySnapshot(MemorySnapshot&&) = delete;
		void operator=(const MemorySnapshot&) = delete;
		void operator=(MemorySnapshot&&) = delete;
		~MemorySnapshot() = default;

		MemorySnapshot(lua_State* L);

		MemorySnapshot(const MemorySnapshot& first, const MemorySnapshot& second);

		void writeToFile(const std::string& fileName) const;
		void writeDigestToFile(const std::string& fileName) const;

	protected:
		bool isMarked(lua_State* L, int index) const;

		bool markPointer(lua_State* L, int index, MarkSource markSource);
		void markObject(lua_State* L, int index, MarkSource markSource);

		void markFunction(lua_State* L, int index, MarkSource markSource);
		void markThread(lua_State* L, int index, MarkSource markSource);
		void markTable(lua_State* L, int index, MarkSource markSource);
		void markLightUserData(lua_State* L, int index, MarkSource markSource);
		void markUserData(lua_State* L, int index, MarkSource markSource);

		void hasWeakKeysAndValues(lua_State* L, int index, bool& weakKeys, bool& weakValues) const;

	protected:
		MarkedMap m_markedMap;
};

} // snapshot
} // lua
} // flat

#endif // FLAT_LUA_MEMORYSNAPSHOT_H


