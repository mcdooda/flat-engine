#include "ui.h"
#include "lua/ui.h"
#include "../../flat.h"

namespace flat
{
namespace sharp
{
namespace ui
{

Ui::Ui(Flat& flat) :
	factory(flat)
{
	root = factory.makeRoot();
	setCursor(CURSOR(ARROW));
	m_defaultCursor = m_cursor;
}

void Ui::setCursor(CursorType cursorType)
{
	if (m_cursor.get() == nullptr || *m_cursor != cursorType)
	{
		m_cursor = std::make_shared<Cursor>(cursorType);
		m_cursor->setCursor();
	}
}

void Ui::setDefaultCursor()
{
	m_cursor = m_defaultCursor;
	m_cursor->setCursor();
}

} // ui
} // sharp
} // flat