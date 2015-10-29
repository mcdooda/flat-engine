#include "widgetfactory.h"
#include "rootwidget.h"
#include "../../flat/game.h"

#include "layouts/fixedlayout.h"
#include "layouts/lineflowlayout.h"

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

Widget* WidgetFactory::makeFixedSize(const geometry::Vector2& size) const
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

} // ui
} // sharp
} // flat
