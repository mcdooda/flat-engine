#include "sharp/ui/cursor.h"

#include "debug/assert.h"

namespace flat
{
namespace sharp
{
namespace ui
{

Cursor::Cursor() :
	m_cursor(nullptr),
	m_cursorType(INVALID_CURSOR)
{
}

Cursor::Cursor(CursorType cursorType) :
	m_cursorType(cursorType)
{
	FLAT_ASSERT(m_cursorType != INVALID_CURSOR);
	m_cursor = SDL_CreateSystemCursor(cursorType);
}

Cursor::Cursor(Cursor&& other) :
	m_cursor(other.m_cursor),
	m_cursorType(other.m_cursorType)
{
	other.m_cursor = nullptr;
	other.m_cursorType = INVALID_CURSOR;
}

Cursor& Cursor::operator=(Cursor&& other)
{
	m_cursor = other.m_cursor;
	other.m_cursor = nullptr;
	m_cursorType = other.m_cursorType;
	other.m_cursorType = INVALID_CURSOR;
	return *this;
}

Cursor::~Cursor()
{
	if (m_cursor != nullptr)
	{
		SDL_FreeCursor(m_cursor);
	}
}

bool Cursor::operator==(CursorType cursorType) const
{
	return m_cursorType == cursorType;
}

bool Cursor::operator!=(CursorType cursorType) const
{
	return m_cursorType != cursorType;
}

void Cursor::setCursor() const
{
	FLAT_ASSERT(m_cursor != nullptr && m_cursorType != INVALID_CURSOR);
	SDL_SetCursor(m_cursor);
}

} // ui
} // sharp
} // flat