#ifndef FLAT_SHARP_UI_CURSOR_H
#define FLAT_SHARP_UI_CURSOR_H

#include <SDL2/SDL.h>

#define CURSOR(type) SDL_SYSTEM_CURSOR_##type

namespace flat
{
namespace sharp
{
namespace ui
{

using CursorType = SDL_SystemCursor;
static constexpr CursorType INVALID_CURSOR = CursorType(-1);

class Cursor
{
	public:
		Cursor();
		Cursor(CursorType cursorType);
		Cursor(const Cursor&) = delete;
		Cursor& operator=(const Cursor&) = delete;
		Cursor(Cursor&& other);
		Cursor& operator=(Cursor&& other);
		~Cursor();

		bool operator==(CursorType cursorType) const;
		bool operator!=(CursorType cursorType) const;

		void setCursor() const;

	private:
		SDL_Cursor* m_cursor;
		CursorType m_cursorType;
};

} // ui
} // sharp
} // flat

#endif // FLAT_SHARP_UI_CURSOR_H


