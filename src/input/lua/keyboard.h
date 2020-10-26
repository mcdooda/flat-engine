#ifndef FLAT_INPUT_LUA_KEYBOARD_H
#define FLAT_INPUT_LUA_KEYBOARD_H

struct lua_State;

namespace flat::input::lua::keyboard
{

int open(lua_State* L);

int l_Keyboard_isKeyPressed(lua_State* L);

} // flat::input::lua::keyboard

#endif // FLAT_INPUT_LUA_KEYBOARD_H



