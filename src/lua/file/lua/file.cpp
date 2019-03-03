#include "file.h"
#include "../regularfile.h"
#include "../directory.h"
#include "../../sharedcppreference.h"

namespace flat
{
namespace lua
{
namespace file
{
namespace lua
{

using LuaFile = flat::lua::SharedCppReference<File>;

int open(Lua& lua)
{
	lua_State* L = lua.state;
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	static const luaL_Reg File_lib_m[] = {
		{"getPath",              l_File_getPath},
		{"getParentPath",        l_File_getParentPath},
		{"getFileName",          l_File_getFileName},
		{"getStem",              l_File_getStem},
		{"getShortStem",         l_File_getShortStem},
		{"getExtension",         l_File_getExtension},
		{"getFullExtension",     l_File_getFullExtension},
		{"isRegularFile",        l_File_isRegularFile},
		{"isDirectory",          l_File_isDirectory},

		{"eachSubFile",          l_Directory_eachSubFile},
		{"getSubFiles",          l_Directory_getSubFiles},

		{"eachSubFileRecursive", l_Directory_eachSubFileRecursive},
		{"getSubFilesRecursive", l_Directory_getSubFilesRecursive},

		{nullptr, nullptr}
	};
	lua.registerClass<LuaFile>("flat.File", File_lib_m);

	lua_getglobal(L, "flat");

	// constructor: flat.File()
	lua_pushcfunction(L, l_File);
	lua_setfield(L, -2, "File");

	lua_pushcfunction(L, l_Directory);
	lua_setfield(L, -2, "Directory");

	lua_pop(L, 1);

	return 0;
}

int l_File(lua_State* L)
{
	const char* path = luaL_checkstring(L, 1);
	std::shared_ptr<File> file = File::open(path);
	pushFile(L, file);
	return 1;
}

int l_Directory(lua_State* L)
{
	const char* path = luaL_checkstring(L, 1);
	std::shared_ptr<Directory> directory = Directory::open(path);
	pushFile(L, directory);
	return 1;
}

int l_Directory_eachSubFile(lua_State* L)
{
	Directory& directory = getFileOfType<Directory>(L, 1);
	luaL_checktype(L, 2, LUA_TFUNCTION);
	directory.eachSubFile(
		[L](const std::shared_ptr<File>& file)
		{
			FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
			lua_pushvalue(L, 2);
			pushFile(L, file);
			lua_call(L, 1, 0);
		}
	);
	return 0;
}

int l_Directory_getSubFiles(lua_State* L)
{
	Directory& directory = getFileOfType<Directory>(L, 1);
	std::vector<std::shared_ptr<File>> files;
	directory.getSubFiles(files);
	const int fileCount = static_cast<int>(files.size());
	lua_createtable(L, fileCount, 0);
	for (int i = 0; i < fileCount; ++i)
	{
		pushFile(L, files[i]);
		lua_rawseti(L, -2, i);
	}
	return 1;
}

int l_Directory_eachSubFileRecursive(lua_State* L)
{
	Directory& directory = getFileOfType<Directory>(L, 1);
	luaL_checktype(L, 2, LUA_TFUNCTION);
	directory.eachSubFileRecursive(
		[L](const std::shared_ptr<File>& file)
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
		lua_pushvalue(L, 2);
		pushFile(L, file);
		lua_call(L, 1, 0);
	}
	);
	return 0;
}

int l_Directory_getSubFilesRecursive(lua_State* L)
{
	Directory& directory = getFileOfType<Directory>(L, 1);
	std::vector<std::shared_ptr<File>> files;
	directory.getSubFilesRecursive(files);
	const int fileCount = static_cast<int>(files.size());
	lua_createtable(L, fileCount, 0);
	for (int i = 0; i < fileCount; ++i)
	{
		pushFile(L, files[i]);
		lua_rawseti(L, -2, i);
	}
	return 1;
}

int l_File_getPath(lua_State* L)
{
	File& file = getFile(L, 1);
	lua_pushstring(L, file.getPath().c_str());
	return 1;
}

int l_File_getParentPath(lua_State* L)
{
	File& file = getFile(L, 1);
	lua_pushstring(L, file.getParentPath().c_str());
	return 1;
}

int l_File_getFileName(lua_State* L)
{
	File& file = getFile(L, 1);
	lua_pushstring(L, file.getFileName().c_str());
	return 1;
}

int l_File_getStem(lua_State* L)
{
	File& file = getFile(L, 1);
	lua_pushstring(L, file.getStem().c_str());
	return 1;
}

int l_File_getShortStem(lua_State* L)
{
	File& file = getFile(L, 1);
	lua_pushstring(L, file.getShortStem().c_str());
	return 1;
}

int l_File_getExtension(lua_State* L)
{
	File& file = getFile(L, 1);
	lua_pushstring(L, file.getExtension().c_str());
	return 1;
}

int l_File_getFullExtension(lua_State* L)
{
	File& file = getFile(L, 1);
	lua_pushstring(L, file.getFullExtension().c_str());
	return 1;
}

int l_File_isRegularFile(lua_State* L)
{
	File& file = getFile(L, 1);
	lua_pushboolean(L, file.is<RegularFile>() && file.isValid());
	return 1;
}

int l_File_isDirectory(lua_State* L)
{
	File& file = getFile(L, 1);
	lua_pushboolean(L, file.is<Directory>() && file.isValid());
	return 1;
}

File& getFile(lua_State* L, int index)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
	return LuaFile::get(L, index);
}

void pushFile(lua_State* L, const std::shared_ptr<File>& file)
{
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 1);
	if (file)
	{
		LuaFile::pushNew(L, file);
	}
	else
	{
		lua_pushnil(L);
	}
}

template <class T>
T& getFileOfType(lua_State* L, int index)
{
	File& file = getFile(L, index);
	if (!file.is<T>() || !file.isValid())
	{
		luaL_error(L, "Wrong file type");
	}
	return file.as<T>();
}

} // lua
} // file
} // lua
} // flat