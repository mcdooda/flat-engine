#ifndef FLAT_LUA_TABLE_H
#define FLAT_LUA_TABLE_H

#include <vector>
#include "lua/push.h"

namespace flat::lua::table
{

template <typename ValueType>
inline std::vector<ValueType> getArray(lua_State* L, int arg)
{
	luaL_checktype(L, arg, LUA_TTABLE);
	size_t numControlPoints = lua_rawlen(L, arg);
	std::vector<ValueType> array;
	array.reserve(numControlPoints);
	for (int i = 1; i <= numControlPoints; ++i)
	{
		lua_rawgeti(L, arg, i);
		array.push_back(getValue<ValueType>(L, -1));
		lua_pop(L, 1);
	}
	return array;
}

template <typename ValueType>
struct KeyValuePair
{
	const char* key;
	ValueType value;
};

template <typename ValueType>
inline int pushTable(lua_State* L, const KeyValuePair<ValueType>* values)
{
	int tableLength;
	for (tableLength = 0; values[tableLength].key; ++tableLength);
	lua_createtable(L, 0, tableLength);
	for (int i = 0; i < tableLength; ++i)
	{
		pushValue(L, values[i].value);
		lua_setfield(L, -2, values[i].key);
	}
	return 1;
}

template <typename ValueType>
inline int pushVector(lua_State* L, const std::vector<ValueType>& values)
{
	const int size = static_cast<int>(values.size());
	lua_createtable(L, size, 0);
	for (int i = 0; i < size; ++i)
	{
		pushValue(L, values[i]);
		lua_rawseti(L, -2, i + 1);
	}
	return 1;
}

void clear(lua_State* L, int index);

bool inheritsFrom(lua_State* L, int classIndex, int superClassIndex);
bool containsAllFields(lua_State* L, int classIndex, int superClassIndex);
bool implementsInterface(lua_State* L, int objectIndex, int interfaceIndex);


} // flat::lua::table

#endif // FLAT_LUA_TABLE_H



