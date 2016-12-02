#ifndef FLAT_LUA_TABLE_H
#define FLAT_LUA_TABLE_H

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
	FLAT_ASSERT_MSG(false, "Cannot lua_push* this type");
}

template <>
inline void pushValue<int>(lua_State* L, int value)
{
	lua_pushinteger(L, value);
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

} // table
} // lua
} // flat

#endif // FLAT_LUA_TABLE_H



