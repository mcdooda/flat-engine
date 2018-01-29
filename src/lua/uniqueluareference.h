#ifndef FLAT_LUA_UNIQUELUAREFERENCE_H
#define FLAT_LUA_UNIQUELUAREFERENCE_H

#include "luareference.h"

namespace flat
{
namespace lua
{

	// UniqueLuaReference is used to hold a non-shared lua object through c++

	template <int LuaType>
	class UniqueLuaReference : public LuaReference<LuaType>
	{
		using Super = LuaReference<LuaType>;
		public:
			UniqueLuaReference() = default;

			UniqueLuaReference(lua_State* L, int index) : Super(L, index)
			{
				
			}

			UniqueLuaReference(const UniqueLuaReference&) = delete;

			UniqueLuaReference(UniqueLuaReference&& other)
			{
				m_luaState = other.m_luaState;
				m_luaReference = other.m_luaReference;
				other.m_luaState = nullptr;
				other.m_luaReference = LUA_NOREF;
			}

			void operator=(const UniqueLuaReference&) = delete;

			void operator=(UniqueLuaReference&& other)
			{
				m_luaState = other.m_luaState;
				m_luaReference = other.m_luaReference;
				other.m_luaState = nullptr;
				other.m_luaReference = LUA_NOREF;
			}
	};

} // lua
} // flat

#endif // FLAT_LUA_UNIQUELUAREFERENCE_H



