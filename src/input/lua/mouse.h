#ifndef FLAT_INPUT_LUA_MOUSE_H
#define FLAT_INPUT_LUA_MOUSE_H

struct lua_State;

namespace flat
{
namespace input
{
namespace lua
{
namespace mouse
{

int open(lua_State* L);

int l_Mouse_getPosition(lua_State* L);

} // mouse
} // lua
} // input
} // flat

#endif // FLAT_INPUT_LUA_MOUSE_H



