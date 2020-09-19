#ifndef FLAT_LUA_PATH_H
#define FLAT_LUA_PATH_H

#include <string>

namespace flat::lua
{

void formatPathToLua(std::string& path);
void formatPathFromLua(std::string& path);

} // game::lua

#endif // FLAT_LUA_PATH_H