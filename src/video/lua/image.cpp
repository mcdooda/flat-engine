#include <lua5.3/lua.hpp>
#include "../../flat/game.h"
#include "image.h"

namespace flat
{
namespace video
{
namespace lua
{
namespace image
{

int open(lua_State* L)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	// Image static methods
	static const luaL_Reg Image_lib_s[] = {
		{ "getSize", l_Image_getSize },

		{ nullptr, nullptr }
	};

	luaL_newlib(L, Image_lib_s);
	lua_setglobal(L, "Image");

	return 0;
}

int l_Image_getSize(lua_State* L)
{
	const char* texturePath = luaL_checkstring(L, 1);
	Flat& flat = flat::lua::getFlat(L);
	std::shared_ptr<const FileTexture> texture = flat.video->getTexture(texturePath);
	lua_pushinteger(L, static_cast<lua_Integer>(texture->getSize().x));
	lua_pushinteger(L, static_cast<lua_Integer>(texture->getSize().y));
	return 2;
}

} // image
} // lua
} // video
} // flat


