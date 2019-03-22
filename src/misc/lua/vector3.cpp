#include "misc/lua/vector3.h"
#include "misc/lua/vector2.h"

#include "lua/sharedcppreference.h"

namespace flat
{
namespace lua
{

using LuaVector3 = flat::lua::SharedCppValue<Vector3>;

int openVector3(Lua& lua)
{
	lua_State* L = lua.state;
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	static const luaL_Reg Vector3_lib_m[] = {
		{"x",             l_Vector3_x},
		{"y",             l_Vector3_y},
		{"z",             l_Vector3_z},

		{"length",        l_Vector3_length},
		{"length2",       l_Vector3_length2},

		{"getNormalized", l_Vector3_getNormalized},

		{"toVector2",     l_Vector3_toVector2},

		{"__add",         l_Vector3_add},
		{"__sub",         l_Vector3_sub},

		{"__mul",         l_Vector3_mul},
		{"__div",         l_Vector3_div},

		{"reflect",       l_Vector3_reflect},
		{"dot",           l_Vector3_dot},

		{"__tostring",    l_Vector3_tostring},

		{nullptr, nullptr}
	};
	lua.registerClass<LuaVector3>("flat.Vector3", Vector3_lib_m);

	// constructor: flat.Vector3(x, y, z)
	lua_getglobal(L, "flat");
	lua_pushcfunction(L, l_Vector3);
	lua_setfield(L, -2, "Vector3");

	lua_pop(L, 1);

	return 0;
}

int l_Vector3(lua_State* L)
{
	float x = static_cast<float>(luaL_checknumber(L, 1));
	float y = static_cast<float>(luaL_checknumber(L, 2));
	float z = static_cast<float>(luaL_checknumber(L, 3));
	pushVector3(L, Vector3(x, y, z));
	return 1;
}

int l_Vector3_x(lua_State* L)
{
	Vector3& vector3 = getVector3(L, 1);
	if (lua_isnoneornil(L, 2))
	{
		lua_pushnumber(L, vector3.x);
		return 1;
	}
	else
	{
		vector3.x = static_cast<float>(luaL_checknumber(L, 2));
		return 0;
	}
}

int l_Vector3_y(lua_State* L)
{
	Vector3& vector3 = getVector3(L, 1);
	if (lua_isnoneornil(L, 2))
	{
		lua_pushnumber(L, vector3.y);
		return 1;
	}
	else
	{
		vector3.y = static_cast<float>(luaL_checknumber(L, 2));
		return 0;
	}
}

int l_Vector3_z(lua_State* L)
{
	Vector3& vector3 = getVector3(L, 1);
	if (lua_isnoneornil(L, 2))
	{
		lua_pushnumber(L, vector3.z);
		return 1;
	}
	else
	{
		vector3.z = static_cast<float>(luaL_checknumber(L, 2));
		return 0;
	}
}

int l_Vector3_length(lua_State* L)
{
	Vector3& vector3 = getVector3(L, 1);
	lua_pushnumber(L, length(vector3));
	return 1;
}

int l_Vector3_length2(lua_State* L)
{
	Vector3& vector3 = getVector3(L, 1);
	lua_pushnumber(L, length2(vector3));
	return 1;
}

int l_Vector3_getNormalized(lua_State* L)
{
	Vector3& vector3 = getVector3(L, 1);
	pushVector3(L, normalize(vector3));
	return 1;
}

int l_Vector3_toVector2(lua_State* L)
{
	Vector3& vector3 = getVector3(L, 1);
	pushVector2(L, Vector2(vector3));
	return 1;
}

int l_Vector3_add(lua_State* L)
{
	pushVector3(L, getVector3(L, 1) + getVector3(L, 2));
	return 1;
}

int l_Vector3_sub(lua_State* L)
{
	pushVector3(L, getVector3(L, 1) - getVector3(L, 2));
	return 1;
}

int l_Vector3_mul(lua_State* L)
{
	Vector3& vector3 = getVector3(L, 1);
	float factor = static_cast<float>(luaL_checknumber(L, 2));
	pushVector3(L, vector3 * factor);
	return 1;
}

int l_Vector3_div(lua_State* L)
{
	Vector3& vector3 = getVector3(L, 1);
	float div = static_cast<float>(luaL_checknumber(L, 2));
	pushVector3(L, vector3 / div);
	return 1;
}

int l_Vector3_reflect(lua_State* L)
{
	Vector3& incident = getVector3(L, 1);
	Vector3& normal = getVector3(L, 2);
	pushVector3(L, flat::reflect(incident, normal));
	return 1;
}

int l_Vector3_dot(lua_State* L)
{
	Vector3& vector3 = getVector3(L, 1);
	Vector3& other = getVector3(L, 2);
	lua_pushnumber(L, dot(vector3, other));
	return 1;
}

int l_Vector3_tostring(lua_State* L)
{
	Vector3& vector3 = getVector3(L, 1);
	lua_pushfstring(L, "flat.Vector3(%f,%f,%f)", vector3.x, vector3.y, vector3.z);
	return 1;
}

Vector3& getVector3(lua_State* L, int index)
{
	return LuaVector3::get(L, index);
}

void pushVector3(lua_State* L, const Vector3& vector3)
{
	LuaVector3::pushNew(L, vector3);
}

} // lua
} // game