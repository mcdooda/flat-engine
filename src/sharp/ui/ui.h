#ifndef FLAT_SHARP_UI_UI_H
#define FLAT_SHARP_UI_UI_H

#include <memory>
#include "rootwidget.h"
#include "widgetfactory.h"

namespace flat
{
class Flat;
namespace sharp
{
namespace ui
{

class Ui
{
	public:
		Ui(Flat& flat);

	public:
		WidgetFactory factory;
		std::shared_ptr<sharp::ui::RootWidget> root;
};

} // ui
} // sharp
} // flat

#endif // FLAT_SHARP_UI_UI_H


