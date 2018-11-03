#include "notify.h"
#include "../../flat.h"

namespace flat
{
namespace sharp
{
namespace notify
{

Notify::Notify(Flat& flat) :
	m_flat(flat)
{

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

void Notify::success(const std::string& message)
{
	m_success.callFunctionP(message);
}

void Notify::warn(const std::string& message)
{
	m_warn.callFunctionP(message);
}

void Notify::error(const std::string& message)
{
	m_error.callFunctionP(message);
}

void Notify::info(const std::string& message)
{
	m_info.callFunctionP(message);
}

} // notify
} // sharp
} // flat


