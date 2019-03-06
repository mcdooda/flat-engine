#ifndef FLAT_SHARP_UI_UI_H
#define FLAT_SHARP_UI_UI_H

#include <memory>

#include "sharp/ui/widgetfactory.h"
#include "sharp/ui/cursor.h"

namespace flat
{
class Flat;
namespace sharp
{
namespace ui
{
class RootWidget;

class Ui
{
	public:
		Ui(Flat& flat);

		void reset();

		void setCursor(CursorType cursorType);
		void setDefaultCursor();

		void setCursorOverride(CursorType cursorType);
		void resetCursorOverride();

	public:
		WidgetFactory factory;
		std::shared_ptr<sharp::ui::RootWidget> root;

	private:
		std::shared_ptr<Cursor> m_cursor;
		std::shared_ptr<Cursor> m_defaultCursor;
		std::shared_ptr<Cursor> m_cursorOverride;
};

} // ui
} // sharp
} // flat

#endif // FLAT_SHARP_UI_UI_H


