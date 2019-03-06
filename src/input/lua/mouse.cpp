#include <lua5.3/lua.hpp>

#include "input/lua/mouse.h"

#include "flat.h"

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
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	static const luaL_Reg Mouse_lib_s[] = {
		{"getPosition",      l_Mouse_getPosition},

		{"setCursor",        l_Mouse_setCursor},
		{"setDefaultCursor", l_Mouse_setDefaultCursor},

		{nullptr, nullptr}
	};
	luaL_newlib(L, Mouse_lib_s);

	lua_pushstring(L, "Cursor");
	static const flat::lua::table::KeyValuePair<int> cursorTypesTable[] = {
		{"ARROW",     CURSOR(ARROW)},
		{"IBEAM",     CURSOR(IBEAM)},
		{"WAIT",      CURSOR(WAIT)},
		{"CROSSHAIR", CURSOR(CROSSHAIR)},
		{"WAITARROW", CURSOR(WAITARROW)},
		{"SIZENWSE",  CURSOR(SIZENWSE)},
		{"SIZENESW",  CURSOR(SIZENESW)},
		{"SIZEWE",    CURSOR(SIZEWE)},
		{"SIZENS",    CURSOR(SIZENS)},
		{"SIZEALL",   CURSOR(SIZEALL)},
		{"NO",        CURSOR(NO)},
		{"HAND",      CURSOR(HAND)},

		{nullptr, 0},
	};
	flat::lua::table::pushTable(L, cursorTypesTable);
	lua_settable(L, -3);

	lua_setglobal(L, "Mouse");
	
	return 0;
}

int l_Mouse_getPosition(lua_State* L)
{
	const Vector2& position = flat::lua::getFlat(L).input->mouse->getPosition();
	lua_pushnumber(L, position.x);
	lua_pushnumber(L, position.y);
	return 2;
}

int l_Mouse_setCursor(lua_State* L)
{
	flat::sharp::ui::CursorType cursorType = static_cast<flat::sharp::ui::CursorType>(luaL_checkinteger(L, 1));
	flat::lua::getFlat(L).ui->setCursorOverride(cursorType);
	return 0;
}

int l_Mouse_setDefaultCursor(lua_State* L)
{
	flat::lua::getFlat(L).ui->resetCursorOverride();
	return 0;
}

} // mouse
} // lua
} // input
} // flat
