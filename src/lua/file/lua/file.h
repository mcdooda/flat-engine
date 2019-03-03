#ifndef FLAT_LUA_FILE_LUA_FILE_H
#define FLAT_LUA_FILE_LUA_FILE_H

#include "../../lua.h"

namespace flat
{
namespace lua
{
namespace file
{
class File;
namespace lua
{

int open(Lua& lua);

int l_File(lua_State* L);

int l_File_getPath(lua_State* L);
int l_File_getParentPath(lua_State* L);
int l_File_getFileName(lua_State* L);
int l_File_getStem(lua_State* L);
int l_File_getShortStem(lua_State* L);
int l_File_getExtension(lua_State* L);
int l_File_getFullExtension(lua_State* L);
int l_File_isRegularFile(lua_State* L);
int l_File_isDirectory(lua_State* L);

int l_Directory(lua_State* L);
int l_Directory_eachSubFile(lua_State* L);
int l_Directory_getSubFiles(lua_State* L);
int l_Directory_eachSubFileRecursive(lua_State* L);
int l_Directory_getSubFilesRecursive(lua_State* L);

File& getFile(lua_State* L, int index);
void pushFile(lua_State* L, const std::shared_ptr<File>& file);

template <class T>
T& getFileOfType(lua_State* L, int index);

} // lua
} // file
} // lua
} // flat

#endif // FLAT_LUA_FILE_LUA_FILE_H