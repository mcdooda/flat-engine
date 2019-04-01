#ifndef FLAT_SHARP_NOTITY_NOTIFY_H
#define FLAT_SHARP_NOTITY_NOTIFY_H

#include "lua/uniqueluareference.h"

namespace flat
{
class Flat;
namespace sharp
{
namespace notify
{

class Notify
{
	public:
		Notify(Flat& flat);

		void reset();

		void success(const std::string& message) const;
		void warn(const std::string& message) const;
		void error(const std::string& message) const;
		void info(const std::string& message) const;

	private:
		Flat& m_flat;

		lua::UniqueLuaReference<LUA_TFUNCTION> m_success;
		lua::UniqueLuaReference<LUA_TFUNCTION> m_warn;
		lua::UniqueLuaReference<LUA_TFUNCTION> m_error;
		lua::UniqueLuaReference<LUA_TFUNCTION> m_info;
};

} // ui
} // sharp
} // flat

#endif // FLAT_SHARP_NOTITY_NOTIFY_H


