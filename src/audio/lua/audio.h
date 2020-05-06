#ifndef FLAT_AUDIO_LUA_AUDIO_H
#define FLAT_AUDIO_LUA_AUDIO_H

struct lua_State;
namespace flat::lua { class Lua; }

namespace flat::audio::lua
{

int open(flat::lua::Lua& lua);

int l_flat_audio_loadChunk(lua_State* L);
int l_flat_audio_playChunk(lua_State* L);

} // flat::audio::lua

#endif // FLAT_AUDIO_LUA_AUDIO_H