#include "widgetfactory.h"
#include "rootwidget.h"
#include "textwidget.h"
#include "../../flat/game.h"

#include "layouts/fixedlayout.h"
#include "layouts/lineflowlayout.h"
#include "layouts/columnflowlayout.h"

namespace flat
{
namespace sharp
{
namespace ui
{

WidgetFactory::WidgetFactory(Game& game) :
	m_game(game)
{
	
}

RootWidget* WidgetFactory::makeRoot() const
{
	return new RootWidget(m_game);
}

Widget* WidgetFactory::makeImage(const std::string& fileName) const
{
	std::shared_ptr<const video::FileTexture> texture = m_game.video->getTexture(fileName);
	Widget* widget = makeFixedSize(texture->getSize());
	widget->setBackground(texture);
	return widget;
}

Widget* WidgetFactory::makeFixedSize(const Vector2& size) const
{
	Widget* widget = new WidgetImpl<FixedLayout>();
	widget->setSize(size);
	return widget;
}

Widget* WidgetFactory::makeLineFlow() const
{
	Widget* widget = new WidgetImpl<LineFlowLayout>();
	return widget;
}

Widget* WidgetFactory::makeColumnFlow() const
{
	Widget* widget = new WidgetImpl<ColumnFlowLayout>();
	return widget;
}

Widget* WidgetFactory::makeText(const std::string& text, const std::string& fileName, int fontSize) const
{
	std::shared_ptr<const video::font::Font> font = m_game.video->getFont(fileName, fontSize);
	TextWidget* widget = new TextWidget(font);
	widget->setText(text);
	return widget;
}

} // ui
} // sharp
} // flat
