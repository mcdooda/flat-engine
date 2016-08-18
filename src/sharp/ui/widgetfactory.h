#ifndef FLAT_SHARP_UI_WIDGETFACTORY_H
#define FLAT_SHARP_UI_WIDGETFACTORY_H

#include <string>

namespace flat
{
class Game;

namespace geometry
{
class Vector2;
}

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
class Widget;
class RootWidget;

class WidgetFactory
{
	public:
		WidgetFactory(Game& game);
		
		RootWidget* makeRoot() const;
		Widget* makeImage(const std::string& fileName) const;
		Widget* makeFixedSize(const geometry::Vector2& size) const;
		Widget* makeLineFlow() const;
		Widget* makeColumnFlow() const;
		Widget* makeText(const std::string& text, const std::string& fileName, int fontSize) const;
		
	private:
		Game& m_game;
};

} // ui
} // sharp
} // flat

#endif // FLAT_SHARP_UI_WIDGETFACTORY_H


