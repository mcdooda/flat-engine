#ifndef FLAT_INPUT_LUA_GAMEPADS_H
#define FLAT_INPUT_LUA_GAMEPADS_H

struct lua_State;

namespace flat
{
namespace input
{
namespace lua
{
namespace gamepads
{

int open(lua_State* L);

int l_Gamepads_isPressed(lua_State* L);
int l_Gamepads_isJustPressed(lua_State* L);
int l_Gamepads_isJustReleased(lua_State* L);

int l_Gamepads_hasAxisJustMoved(lua_State* L);
int l_Gamepads_getAxisValue(lua_State* L);

int l_Gamepads_getLeftStickValue(lua_State* L);
int l_Gamepads_getRightStickValue(lua_State* L);

} // gamepads
} // lua
} // input
} // flat

#endif // FLAT_INPUT_LUA_GAMEPADS_H



