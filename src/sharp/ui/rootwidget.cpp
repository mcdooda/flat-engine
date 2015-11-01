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
		handleMouseLeave();
		m_mouseOverWidget = mouseOverWidget;
		handleMouseEnter();
	}

	if (m_game.input->mouse->isPressed(M(LEFT)))
		handleClick();
}

void RootWidget::handleClick()
{
	bool eventHandled = false;
	Widget* widget = m_mouseOverWidget;
	while (widget && !eventHandled)
	{
		widget->click(widget, eventHandled);
		widget = widget->getParent();
	}
}

void RootWidget::handleMouseEnter()
{
	if (m_mouseOverWidget)
	{
		m_mouseOverWidget->m_mouseOver = true;
		m_mouseOverWidget->mouseEnter(m_mouseOverWidget);
	}
}

void RootWidget::handleMouseLeave()
{
	if (m_mouseOverWidget)
	{
		m_mouseOverWidget->m_mouseOver = false;
		m_mouseOverWidget->mouseLeave(m_mouseOverWidget);
	}
}

} // ui
} // sharp
} // flat
