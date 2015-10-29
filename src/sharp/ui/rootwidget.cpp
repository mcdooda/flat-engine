#include "rootwidget.h"
#include "../../video/window.h"
#include "../../flat/game.h"

namespace flat
{
namespace sharp
{
namespace ui
{

RootWidget::RootWidget(Game& game) : Super(),
	m_game(game),
	m_mouseOverWidget(nullptr)
{

}

void RootWidget::draw(const flat::util::RenderSettings& renderSettings) const
{
	drawChildren(renderSettings);
}

void RootWidget::updateInput()
{
	geometry::Vector2 mousePosition = m_game.input->mouse->getPosition();
	Widget* mouseOverWidget = getMouseOverWidget(mousePosition);
	if (mouseOverWidget == this)
		mouseOverWidget = nullptr;

	if (mouseOverWidget != m_mouseOverWidget)
	{
		if (m_mouseOverWidget)
			m_mouseOverWidget->onMouseLeave();

		if (mouseOverWidget)
			mouseOverWidget->onMouseEnter();

		m_mouseOverWidget = mouseOverWidget;
	}
}

void RootWidget::handleClick()
{
	bool eventHandled = false;
	Widget* widget = m_mouseOverWidget;
	while (widget && !eventHandled)
	{
		widget->onClick(widget, eventHandled);
		widget = widget->getParent();
	}
}

} // ui
} // sharp
} // flat
