#include "vector2.h"
#include "vector3.h"
#include "../../lua/sharedcppreference.h"

namespace flat
{
namespace lua
{

using LuaVector2 = flat::lua::SharedCppValue<Vector2>;

int openVector2(Lua& lua)
{
	lua_State* L = lua.state;
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	static const luaL_Reg Vector2_lib_m[] = {
		{"x",             l_Vector2_x},
		{"y",             l_Vector2_y},

		{"length",        l_Vector2_length},
		{"length2",       l_Vector2_length2},

		{"getNormalized", l_Vector2_getNormalized},

		{"toVector3",     l_Vector2_toVector3},

		{"__add",         l_Vector2_add},
		{"__sub",         l_Vector2_sub},

		{"__mul",         l_Vector2_mul},

		{"reflect",       l_Vector2_reflect},

		{"__tostring",    l_Vector2_tostring},

		{nullptr, nullptr}
	};
	lua.registerClass<LuaVector2>("flat.Vector2", Vector2_lib_m);

	// constructor: flat.Vector2(x, y)
	lua_getglobal(L, "flat");
	lua_pushcfunction(L, l_Vector2);
	lua_setfield(L, -2, "Vector2");

	lua_pop(L, 1);

	return 0;
}

int l_Vector2(lua_State* L)
{
	float x = static_cast<float>(luaL_checknumber(L, 1));
	float y = static_cast<float>(luaL_checknumber(L, 2));
	pushVector2(L, Vector2(x, y));
	return 1;
}

int l_Vector2_x(lua_State* L)
{
	Vector2& vector2 = getVector2(L, 1);
	if (lua_isnoneornil(L, 2))
	{
		lua_pushnumber(L, vector2.x);
		return 1;
	}
	else
	{
		vector2.x = static_cast<float>(luaL_checknumber(L, 2));
		return 0;
	}
}

int l_Vector2_y(lua_State* L)
{
	Vector2& vector2 = getVector2(L, 1);
	if (lua_isnoneornil(L, 2))
	{
		lua_pushnumber(L, vector2.y);
		return 1;
	}
	else
	{
		vector2.y = static_cast<float>(luaL_checknumber(L, 2));
		return 0;
	}
}

int l_Vector2_length(lua_State* L)
{
	Vector2& vector2 = getVector2(L, 1);
	lua_pushnumber(L, length(vector2));
	return 1;
}

int l_Vector2_length2(lua_State* L)
{
	Vector2& vector2 = getVector2(L, 1);
	lua_pushnumber(L, length2(vector2));
	return 1;
}

int l_Vector2_getNormalized(lua_State* L)
{
	Vector2& vector2 = getVector2(L, 1);
	pushVector2(L, normalize(vector2));
	return 1;
}

int l_Vector2_toVector3(lua_State* L)
{
	Vector2& vector2 = getVector2(L, 1);
	float z = static_cast<float>(luaL_optnumber(L, 2, 0.f));
	pushVector3(L, Vector3(vector2, z));
	return 1;
}

int l_Vector2_add(lua_State* L)
{
	pushVector2(L, getVector2(L, 1) + getVector2(L, 2));
	return 1;
}

int l_Vector2_sub(lua_State* L)
{
	pushVector2(L, getVector2(L, 1) - getVector2(L, 2));
	return 1;
}

int l_Vector2_mul(lua_State* L)
{
	Vector2& vector2 = getVector2(L, 1);
	float multiplier = static_cast<float>(luaL_checknumber(L, 2));
	pushVector2(L, vector2 * multiplier);
	return 1;
}

int l_Vector2_reflect(lua_State* L)
{
	Vector2& incident = getVector2(L, 1);
	Vector2& normal = getVector2(L, 2);
	pushVector2(L, flat::reflect(incident, normal));
	return 1;
}

int l_Vector2_tostring(lua_State* L)
{
	Vector2& vector2 = getVector2(L, 1);
	lua_pushfstring(L, "flat.Vector2(%f,%f)", vector2.x, vector2.y);
	return 1;
}

Vector2& getVector2(lua_State* L, int index)
{
	return LuaVector2::get(L, index);
}

void pushVector2(lua_State* L, const Vector2& vector2)
{
	LuaVector2::pushNew(L, vector2);
}

} // lua
} // game