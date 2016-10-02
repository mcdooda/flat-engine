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
	m_mouseOverWidget(nullptr),
	m_dirty(true)
{
	setSizePolicy(Widget::SizePolicy::FIXED);
}

void RootWidget::draw(const flat::util::RenderSettings& renderSettings) const
{
	drawChildren(renderSettings);
}

void RootWidget::addDirtyWidget(Widget* widget)
{
	if (m_dirty)
		return;

	std::vector<Widget*>::iterator it = std::find(m_dirtyWidgets.begin(), m_dirtyWidgets.end(), widget);
	if (it == m_dirtyWidgets.end())
	{
		m_dirtyWidgets.push_back(widget);
	}
}

void RootWidget::updateDirtyWidgets()
{
	if (m_dirty)
	{
		FLAT_ASSERT(m_dirtyWidgets.empty());
		fullLayout();
		m_dirty = false;
	}
	else
	{
		for (Widget* widget : m_dirtyWidgets)
		{
			widget->fullLayout();
		}
		m_dirtyWidgets.clear();
	}
}

void RootWidget::setDirty()
{
	m_dirty = true;
	m_dirtyWidgets.clear();
}

void RootWidget::updateInput()
{
	const flat::input::Mouse* mouse = m_game.input->mouse;
	if (mouse->justMoved())
	{
		Vector2 mousePosition = mouse->getPosition();
		Widget* mouseOverWidget = getMouseOverWidget(mousePosition);

		if (mouseOverWidget == this) // root
			mouseOverWidget = nullptr;

		if (mouseOverWidget != m_mouseOverWidget)
		{
			handleMouseLeave();
			m_mouseOverWidget = mouseOverWidget;
			handleMouseEnter();
		}

		handleMouseMove();
	}

	if (mouse->isJustPressed(M(LEFT)))
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

void RootWidget::handleMouseMove()
{
	bool eventHandled = false;
	Widget* widget = m_mouseOverWidget;
	while (widget && !eventHandled)
	{
		widget->mouseMove(widget, eventHandled);
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
