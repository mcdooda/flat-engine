#ifndef FLAT_VIDEO_LUA_IMAGE_H
#define FLAT_VIDEO_LUA_IMAGE_H

struct lua_State;

namespace flat
{
namespace video
{
namespace lua
{
namespace image
{

int open(lua_State* L);

int l_Image_getSize(lua_State* L);

} // image
} // lua
} // video
} // flat

#endif // FLAT_VIDEO_LUA_IMAGE_H


