#include <lua5.3/lua.hpp>
#include <flat.h>
#include "mouse.h"

namespace flat
{
namespace input
{
namespace lua
{
namespace mouse
{

int open(lua_State* L)
{
	static const luaL_Reg Mouse_lib_s[] = {
		{"getPosition", l_Mouse_getPosition},
		
		{nullptr, nullptr}
	};
	luaL_newlib(L, Mouse_lib_s);
	lua_setglobal(L, "Mouse");
	
	return 0;
}

int l_Mouse_getPosition(lua_State* L)
{
	input::Mouse* mouse = flat::lua::getInput(L)->mouse;
	const Vector2& position = mouse->getPosition();
	lua_pushnumber(L, position.x);
	lua_pushnumber(L, position.y);
	return 2;
}

} // mouse
} // lua
} // input
} // flat
