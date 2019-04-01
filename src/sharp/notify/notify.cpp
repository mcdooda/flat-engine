#include "sharp/notify/notify.h"

#include "flat.h"

namespace flat
{
namespace sharp
{
namespace notify
{

Notify::Notify(Flat& flat) :
	m_flat(flat)
{
	reset();
}

void Notify::reset()
{
	lua_State* L = m_flat.lua->state;
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

		m_flat.lua->pushVariable({"flat", "ui", "success"});
		m_success.set(L, -1);

		m_flat.lua->pushVariable({"flat", "ui", "warn"});
		m_warn.set(L, -1);

		m_flat.lua->pushVariable({"flat", "ui", "error"});
		m_error.set(L, -1);

		m_flat.lua->pushVariable({"flat", "ui", "info"});
		m_info.set(L, -1);

		lua_pop(L, 4);
	}
}

void Notify::success(const std::string& message) const
{
	m_success.call(message);
}

void Notify::warn(const std::string& message) const
{
	m_warn.call(message);
}

void Notify::error(const std::string& message) const
{
	m_error.call(message);
}

void Notify::info(const std::string& message) const
{
	m_info.call(message);
}

} // notify
} // sharp
} // flat


