#ifdef _WIN32
#include <windows.h>
#endif

#include "console/console.h"

namespace flat::console
{

ColorType WHITE()
{
#ifdef _WIN32
	return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
#else
	return 0;
#endif
}

ColorType RED()
{
#ifdef _WIN32
	return FOREGROUND_RED;
#else
	return 0;
#endif
}

flat::console::ColorType LIGHT_RED()
{
#ifdef _WIN32
	return FOREGROUND_RED | FOREGROUND_INTENSITY;
#else
	return 0;
#endif
}

ColorType GREEN()
{
#ifdef _WIN32
	return FOREGROUND_GREEN;
#else
	return 0;
#endif
}

ColorType LIGHT_GREEN()
{
#ifdef _WIN32
	return FOREGROUND_GREEN | FOREGROUND_INTENSITY;
#else
	return 0;
#endif
}

ColorType BLUE()
{
#ifdef _WIN32
	return FOREGROUND_BLUE;
#else
	return 0;
#endif
}

ColorType LIGHT_BLUE()
{
#ifdef _WIN32
	return FOREGROUND_BLUE | FOREGROUND_INTENSITY;
#else
	return 0;
#endif
}

ColorScope::ColorScope(ColorType color)
{
	m_previousColor = getConsoleColor();
	setConsoleColor(color);
}

ColorScope::~ColorScope()
{
	setConsoleColor(m_previousColor);
}

void ColorScope::setConsoleColor(ColorType color) const
{
#ifdef _WIN32
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout, color);
#endif
}

ColorType ColorScope::getConsoleColor() const
{
#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO info;
	if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info))
	{
		return WHITE();
	}
	return info.wAttributes;
#endif
}

} // flat::console