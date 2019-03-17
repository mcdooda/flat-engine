#ifndef FLAT_LUA_SLOTPROXY_H
#define FLAT_LUA_SLOTPROXY_H

#include <vector>
#include <functional>

#include "lua/uniqueluareference.h"

#include "misc/slot.h"

namespace flat
{
namespace lua
{

template <typename... T>
class SlotProxy
{
	public:
		SlotProxy() = default;
		SlotProxy(const SlotProxy&) = delete;
		SlotProxy(SlotProxy&&) = delete;
		void operator=(const SlotProxy&) = delete;
		void operator=(SlotProxy&&) = delete;

		~SlotProxy()
		{
			reset();
		}

		void init(Slot<T...>* slot, std::function<void(lua_State*, T...)> pushArgumentsCallback)
		{
			FLAT_ASSERT(slot != nullptr);
			m_slot = slot;
			m_slot->on(this, &SlotProxy<T...>::onCall);
			m_pushArgumentsCallback = pushArgumentsCallback;
		}

		void reset()
		{
			if (m_slot != nullptr)
			{
				m_slot->off(this);
			}

			m_callbacks.clear();
			m_pushArgumentsCallback = nullptr;
			m_slot = nullptr;
		}

		int addCallback(lua_State* L, int index)
		{
			m_callbacks.emplace_back();
			m_callbacks.back().set(L, index);
			return static_cast<int>(m_callbacks.size()) - 1;
		}

		void removeCallback(int index)
		{
			FLAT_ASSERT(0 <= index && index < m_callbacks.size());
			m_callbacks.erase(m_callbacks.begin() + index);
		}

	private:
		bool onCall(T... params)
		{
			for (const flat::lua::UniqueLuaReference<LUA_TFUNCTION>& callback : m_callbacks)
			{
				callback.callFunction(
					[this, &params...](lua_State* L)
					{
						m_pushArgumentsCallback(L, params...);
					}
				);
			}
			return true;
		}

	private:
		std::vector<flat::lua::UniqueLuaReference<LUA_TFUNCTION>> m_callbacks;
		std::function<void(lua_State*, T...)> m_pushArgumentsCallback;
		Slot<T...>* m_slot;
};

} // lua
} // flat

#endif // FLAT_LUA_SLOTPROXY_H


