#include <lua5.3/lua.hpp>
#include <flat/game.h>
#include "gamepads.h"

namespace flat
{
namespace input
{
namespace lua
{
namespace gamepads
{

int open(lua_State* L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	static const luaL_Reg Gamepads_lib_s[] = {
		{"isPressed",          l_Gamepads_isPressed},
		{"isJustPressed",      l_Gamepads_isJustPressed},
		{"isJustReleased",     l_Gamepads_isJustReleased},

		{"hasAxisJustMoved",   l_Gamepads_hasAxisJustMoved},
		{"getAxisValue",       l_Gamepads_getAxisValue},

		{"getLeftStickValue",  l_Gamepads_getLeftStickValue},
		{"getRightStickValue", l_Gamepads_getRightStickValue},

		{nullptr, nullptr}
	};
	luaL_newlib(L, Gamepads_lib_s);

#define GAMEPAD_BUTTON_ENTRY(name) {#name, B(name)},
	lua_pushstring(L, "GamepadButton");
	static const flat::lua::table::KeyValuePair<GamepadButton> gamepadButtonsTable[] = {
		GAMEPAD_BUTTON_ENTRY(A)
		GAMEPAD_BUTTON_ENTRY(B)
		GAMEPAD_BUTTON_ENTRY(X)
		GAMEPAD_BUTTON_ENTRY(Y)
		GAMEPAD_BUTTON_ENTRY(BACK)
		GAMEPAD_BUTTON_ENTRY(GUIDE)
		GAMEPAD_BUTTON_ENTRY(START)
		GAMEPAD_BUTTON_ENTRY(LEFTSTICK)
		GAMEPAD_BUTTON_ENTRY(RIGHTSTICK)
		GAMEPAD_BUTTON_ENTRY(LEFTSHOULDER)
		GAMEPAD_BUTTON_ENTRY(RIGHTSHOULDER)
		GAMEPAD_BUTTON_ENTRY(DPAD_UP)
		GAMEPAD_BUTTON_ENTRY(DPAD_DOWN)
		GAMEPAD_BUTTON_ENTRY(DPAD_LEFT)
		GAMEPAD_BUTTON_ENTRY(DPAD_RIGHT)

		{nullptr, B(INVALID)},
	};
	flat::lua::table::pushTable(L, gamepadButtonsTable);
	lua_settable(L, -3);
#undef GAMEPAD_BUTTON_ENTRY

#define GAMEPAD_AXIS_ENTRY(name) {#name, A(name)},
	lua_pushstring(L, "GamepadAxis");
	static const flat::lua::table::KeyValuePair<GamepadAxis> gamepadAxesTable[] = {
		GAMEPAD_AXIS_ENTRY(LEFTX)
		GAMEPAD_AXIS_ENTRY(LEFTY)
		GAMEPAD_AXIS_ENTRY(RIGHTX)
		GAMEPAD_AXIS_ENTRY(RIGHTY)
		GAMEPAD_AXIS_ENTRY(TRIGGERLEFT)
		GAMEPAD_AXIS_ENTRY(TRIGGERRIGHT)

		{nullptr, A(INVALID)},
	};
	flat::lua::table::pushTable(L, gamepadAxesTable);
	lua_settable(L, -3);
#undef GAMEPAD_AXIS_ENTRY

	lua_setglobal(L, "Gamepads");
	
	return 0;
}

int l_Gamepads_isPressed(lua_State* L)
{
	const flat::input::Gamepads& gamepads = *flat::lua::getFlat(L).input->gamepads;
	GamepadIndex gamepadIndex = static_cast<GamepadIndex>(luaL_checkinteger(L, 1));
	GamepadButton gamepadButton = static_cast<GamepadButton>(luaL_checkinteger(L, 2));
	lua_pushboolean(L, gamepads.isPressed(gamepadIndex, gamepadButton));
	return 1;
}

int l_Gamepads_isJustPressed(lua_State* L)
{
	const flat::input::Gamepads& gamepads = *flat::lua::getFlat(L).input->gamepads;
	GamepadIndex gamepadIndex = static_cast<GamepadIndex>(luaL_checkinteger(L, 1));
	GamepadButton gamepadButton = static_cast<GamepadButton>(luaL_checkinteger(L, 2));
	lua_pushboolean(L, gamepads.isJustPressed(gamepadIndex, gamepadButton));
	return 1;
}

int l_Gamepads_isJustReleased(lua_State* L)
{
	const flat::input::Gamepads& gamepads = *flat::lua::getFlat(L).input->gamepads;
	GamepadIndex gamepadIndex = static_cast<GamepadIndex>(luaL_checkinteger(L, 1));
	GamepadButton gamepadButton = static_cast<GamepadButton>(luaL_checkinteger(L, 2));
	lua_pushboolean(L, gamepads.isJustReleased(gamepadIndex, gamepadButton));
	return 1;
}

int l_Gamepads_hasAxisJustMoved(lua_State* L)
{
	const flat::input::Gamepads& gamepads = *flat::lua::getFlat(L).input->gamepads;
	GamepadIndex gamepadIndex = static_cast<GamepadIndex>(luaL_checkinteger(L, 1));
	GamepadAxis gamepadAxis = static_cast<GamepadAxis>(luaL_checkinteger(L, 2));
	lua_pushboolean(L, gamepads.hasAxisJustMoved(gamepadIndex, gamepadAxis));
	return 1;
}

int l_Gamepads_getAxisValue(lua_State* L)
{
	const flat::input::Gamepads& gamepads = *flat::lua::getFlat(L).input->gamepads;
	GamepadIndex gamepadIndex = static_cast<GamepadIndex>(luaL_checkinteger(L, 1));
	GamepadAxis gamepadAxis = static_cast<GamepadAxis>(luaL_checkinteger(L, 2));
	lua_pushnumber(L, gamepads.getAxisValue(gamepadIndex, gamepadAxis));
	return 1;
}


int l_Gamepads_getLeftStickValue(lua_State* L)
{
	const flat::input::Gamepads& gamepads = *flat::lua::getFlat(L).input->gamepads;
	GamepadIndex gamepadIndex = static_cast<GamepadIndex>(luaL_checkinteger(L, 1));
	flat::Vector2 leftStickValue = gamepads.getLeftStickValue(gamepadIndex);
	lua_pushnumber(L, leftStickValue.x);
	lua_pushnumber(L, leftStickValue.y);
	return 2;
}


int l_Gamepads_getRightStickValue(lua_State* L)
{
	const flat::input::Gamepads& gamepads = *flat::lua::getFlat(L).input->gamepads;
	GamepadIndex gamepadIndex = static_cast<GamepadIndex>(luaL_checkinteger(L, 1));
	flat::Vector2 rightStickValue = gamepads.getRightStickValue(gamepadIndex);
	lua_pushnumber(L, rightStickValue.x);
	lua_pushnumber(L, rightStickValue.y);
	return 2;
}

} // gamepads
} // lua
} // input
} // flat

