#ifndef FLAT_MISC_LUA_VECTOR2_H
#define FLAT_MISC_LUA_VECTOR2_H

#include "../vector.h"

struct lua_State;

namespace flat
{
namespace misc
{
namespace lua
{

int openVector2(lua_State* L);

int l_Vector2(lua_State* L);

int l_Vector2_x(lua_State* L);
int l_Vector2_y(lua_State* L);

int l_Vector2_length(lua_State* L);
int l_Vector2_length2(lua_State* L);

int l_Vector2_add(lua_State* L);
int l_Vector2_sub(lua_State* L);

Vector2& getVector2(lua_State* L, int index);
void pushVector2(lua_State* L, const Vector2& vector2);

} // lua
} // misc
} // game

#endif // FLAT_MISC_LUA_VECTOR2_H


