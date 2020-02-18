#ifndef FLAT_CONSOLE_CONSOLE_H
#define FLAT_CONSOLE_CONSOLE_H

namespace flat::console
{

using ColorType = unsigned short;

ColorType WHITE();
ColorType RED();
ColorType LIGHT_RED();
ColorType GREEN();
ColorType LIGHT_GREEN();
ColorType BLUE();
ColorType LIGHT_BLUE();

class ColorScope
{
public:
	ColorScope(ColorType color);
	~ColorScope();

private:
	void setConsoleColor(ColorType color) const;
	ColorType getConsoleColor() const;

private:
	ColorType m_previousColor;
};

} // flat::console

#define FLAT_CONSOLE_COLOR(color) flat::console::ColorScope consoleColorScope(flat::console:: color ())

#endif FLAT_CONSOLE_CONSOLE_H