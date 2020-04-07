#include "audio.h"
#include "flat.h"

namespace flat::audio::lua
{

using LuaChunk = flat::lua::SharedCppReference<const Chunk>;

int open(flat::lua::Lua& lua)
{
	lua_State* L = lua.state;
	FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

	lua.registerClass<LuaChunk>("flat.audio.Chunk");

	lua_getglobal(L, "flat");

	lua_createtable(L, 0, 1);
	static const luaL_Reg Chunk_lib_f[] = {
		{"loadChunk", l_flat_audio_loadChunk},
		{"playChunk", l_flat_audio_playChunk},

		{nullptr, nullptr}
	};
	luaL_setfuncs(L, Chunk_lib_f, 0);
	lua_setfield(L, -2, "audio");

	lua_pop(L, 1);

	return 0;
}

int l_flat_audio_loadChunk(lua_State* L)
{
	const std::string sampleName = luaL_checkstring(L, 1);
	std::shared_ptr<const Chunk> chunk = flat::lua::getFlat(L).audio->getChunk(sampleName);
	LuaChunk::pushNew(L, chunk);
	return 1;
}

int l_flat_audio_playChunk(lua_State* L)
{
	std::shared_ptr<const Chunk>& chunk = LuaChunk::getSharedPointer(L, 1);
	flat::lua::getFlat(L).audio->playChunk(chunk);
	return 0;
}

} // flat::audio::lua

