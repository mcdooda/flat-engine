#ifndef FLAT_MISC_LUA_VECTOR2_H
#define FLAT_MISC_LUA_VECTOR2_H

#include "misc/vector.h"

#include "lua/push.h"

struct lua_State;

namespace flat
{
namespace lua
{
class Lua;

int openVector2(Lua& lua);

int l_Vector2(lua_State* L);

int l_Vector2_x(lua_State* L);
int l_Vector2_y(lua_State* L);

int l_Vector2_length(lua_State* L);
int l_Vector2_length2(lua_State* L);

int l_Vector2_getNormalized(lua_State* L);

int l_Vector2_toVector3(lua_State* L);

int l_Vector2_add(lua_State* L);
int l_Vector2_sub(lua_State* L);

int l_Vector2_mul(lua_State* L);
int l_Vector2_div(lua_State* L);

int l_Vector2_reflect(lua_State* L);
int l_Vector2_dot(lua_State* L);

int l_Vector2_tostring(lua_State* L);

Vector2& getVector2(lua_State* L, int index);
void pushVector2(lua_State* L, const Vector2& vector2);

} // lua
} // flat

FLAT_LUA_PUSHABLE_TYPE(const flat::Vector2&, flat::lua::pushVector2(L, value))

#endif // FLAT_MISC_LUA_VECTOR2_H


