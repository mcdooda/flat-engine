#ifndef FLAT_LUA_TABLE_H
#define FLAT_LUA_TABLE_H

#include <string>

namespace flat
{
namespace lua
{
namespace table
{

template <typename ValueType>
struct KeyValuePair
{
	const char* key;
	ValueType value;
};

template <typename ValueType>
inline void pushValue(lua_State* L, ValueType value)
{
	FLAT_ASSERT_MSG(false, "Cannot lua_push* type %s", typeid(value).name());
}

template <>
inline void pushValue<int>(lua_State* L, int value)
{
	lua_pushinteger(L, value);
}

template <>
inline void pushValue<float>(lua_State* L, float value)
{
	lua_pushnumber(L, value);
}

template <>
inline void pushValue<bool>(lua_State* L, bool value)
{
	lua_pushboolean(L, value);
}

template <>
inline void pushValue<void*>(lua_State* L, void* value)
{
	lua_pushlightuserdata(L, value);
}

template <>
inline void pushValue<const char*>(lua_State* L, const char* value)
{
	lua_pushstring(L, value);
}

template <>
inline void pushValue<std::string>(lua_State* L, std::string value)
{
	lua_pushstring(L, value.c_str());
}

template <typename ValueType>
inline int pushTable(lua_State* L, const KeyValuePair<ValueType>* values)
{
	int tableLength;
	for (tableLength = 0; values[tableLength].key; ++tableLength);
	lua_createtable(L, 0, tableLength);
	for (int i = 0; i < tableLength; ++i)
	{
		pushValue<ValueType>(L, values[i].value);
		lua_setfield(L, -2, values[i].key);
	}
	return 1;
}

void clear(lua_State* L, int index);

} // table
} // lua
} // flat

#endif // FLAT_LUA_TABLE_H



