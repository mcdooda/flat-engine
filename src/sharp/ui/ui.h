#ifndef FLAT_SHARP_UI_UI_H
#define FLAT_SHARP_UI_UI_H

#include <memory>
#include "widgetfactory.h"
#include "cursor.h"

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

		void setCursor(CursorType cursorType);
		void setDefaultCursor();

	public:
		WidgetFactory factory;
		std::shared_ptr<sharp::ui::RootWidget> root;

	private:
		std::shared_ptr<Cursor> m_cursor;
		std::shared_ptr<Cursor> m_defaultCursor;
};

} // ui
} // sharp
} // flat

#endif // FLAT_SHARP_UI_UI_H


