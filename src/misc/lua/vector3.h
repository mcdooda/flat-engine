#ifndef FLAT_MISC_LUA_VECTOR3_H
#define FLAT_MISC_LUA_VECTOR3_H

#include "misc/vector.h"

#include "lua/push.h"

struct lua_State;

namespace flat
{
namespace lua
{
class Lua;

int openVector3(Lua& lua);

int l_Vector3(lua_State* L);

int l_Vector3_x(lua_State* L);
int l_Vector3_y(lua_State* L);
int l_Vector3_z(lua_State* L);

int l_Vector3_length(lua_State* L);
int l_Vector3_length2(lua_State* L);

int l_Vector3_getNormalized(lua_State* L);

int l_Vector3_toVector2(lua_State* L);

int l_Vector3_add(lua_State* L);
int l_Vector3_sub(lua_State* L);

int l_Vector3_mul(lua_State* L);
int l_Vector3_div(lua_State* L);

int l_Vector3_reflect(lua_State* L);
int l_Vector3_dot(lua_State* L);
int l_Vector3_cross(lua_State* L);

int l_Vector3_tostring(lua_State* L);

Vector3& getVector3(lua_State* L, int index);
void pushVector3(lua_State* L, const Vector3& vector3);

} // lua
} // flat

FLAT_LUA_PUSHABLE_TYPE(const flat::Vector3&, flat::lua::pushVector3(L, value))

#endif // FLAT_MISC_LUA_VECTOR3_H


