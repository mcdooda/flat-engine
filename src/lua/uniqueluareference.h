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
			UniqueLuaReference() : Super() {}

			UniqueLuaReference(lua_State* L, int index) : Super(L, index)
			{
				
			}

			UniqueLuaReference(const UniqueLuaReference& other) = delete;

			UniqueLuaReference(UniqueLuaReference&& other)
			{
				m_luaReference = other.m_luaReference;
				other.m_luaReference = LUA_NOREF;
			}

			inline void push(lua_State* L) const
			{
				FLAT_LUA_EXPECT_STACK_GROWTH(L, 1);
				FLAT_ASSERT(m_luaReference != LUA_NOREF);
				{
					std::shared_lock<std::shared_mutex> lock(referenceMutex);
					lua_rawgeti(L, LUA_REGISTRYINDEX, m_luaReference);
				}
			}

			// make the parent method public
			inline void reset(lua_State* L)
			{
				Super::reset(L);
			}

			inline operator bool() const
			{
				return m_luaReference != LUA_NOREF;
			}
	};

} // lua
} // flat

#endif // FLAT_LUA_UNIQUELUAREFERENCE_H



