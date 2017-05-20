#ifndef FLAT_SHARP_UI_WIDGETFACTORY_H
#define FLAT_SHARP_UI_WIDGETFACTORY_H

#include <memory>
#include <string>
#include "../../misc/vector.h"

namespace flat
{
class Flat;

namespace video
{
namespace font
{
class Font;
}
}

namespace sharp
{
namespace ui
{
class RootWidget;
class Widget;
class TextWidget;
class TextInputWidget;

class WidgetFactory
{
	public:
		WidgetFactory(Flat& flat);
		
		std::shared_ptr<RootWidget> makeRoot() const;
		std::shared_ptr<Widget> makeImage(const std::string& fileName) const;
		std::shared_ptr<Widget> makeFixedSize(const Vector2& size) const;
		std::shared_ptr<Widget> makeLineFlow() const;
		std::shared_ptr<Widget> makeColumnFlow() const;
		std::shared_ptr<TextWidget> makeText(const std::string& text, const std::string& fileName, int fontSize) const;
		std::shared_ptr<TextInputWidget> makeTextInput(const std::string& fileName, int fontSize) const;
		
	private:
		Flat& m_flat;
};

} // ui
} // sharp
} // flat

#endif // FLAT_SHARP_UI_WIDGETFACTORY_H



