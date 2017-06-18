#ifndef FLAT_MISC_LUA_VECTOR3_H
#define FLAT_MISC_LUA_VECTOR3_H

#include "../vector.h"

struct lua_State;

namespace flat
{
namespace misc
{
namespace lua
{

int openVector3(lua_State* L);

int l_Vector3(lua_State* L);

int l_Vector3_x(lua_State* L);
int l_Vector3_y(lua_State* L);
int l_Vector3_z(lua_State* L);

int l_Vector3_length(lua_State* L);
int l_Vector3_length2(lua_State* L);

int l_Vector3_add(lua_State* L);
int l_Vector3_sub(lua_State* L);

Vector3& getVector3(lua_State* L, int index);
void pushVector3(lua_State* L, const Vector3& vector3);

} // lua
} // misc
} // game

#endif // FLAT_MISC_LUA_VECTOR3_H


