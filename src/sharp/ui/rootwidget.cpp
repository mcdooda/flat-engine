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
	m_dirty(true)
{
	setSizePolicy(Widget::SizePolicy::FIXED);
}

void RootWidget::draw(const flat::render::RenderSettings& renderSettings) const
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

bool RootWidget::updateDirtyWidgets()
{
	if (m_dirty)
	{
		FLAT_ASSERT(m_dirtyWidgets.empty());
		fullLayout();
		m_dirty = false;
		return true;
	}
	else
	{
		bool widgetsLayout = !m_dirtyWidgets.empty();
		for (Widget* widget : m_dirtyWidgets)
		{
			widget->fullLayout();
		}
		m_dirtyWidgets.clear();
		return widgetsLayout;
	}
}

void RootWidget::setDirty()
{
	m_dirty = true;
	m_dirtyWidgets.clear();
}

void RootWidget::update()
{
	if (m_game.input->window->isResized())
		fullLayout();

	bool updateMouseOver = updateDirtyWidgets();
	updateInput(updateMouseOver);
}

void RootWidget::updateInput(bool updateMouseOver)
{
	const flat::input::Mouse* mouse = m_game.input->mouse;

	if (updateMouseOver || mouse->justMoved())
	{
		Widget* mouseOverWidget = getMouseOverWidget(mouse->getPosition());
		if (mouseOverWidget == this) // root
		{
			mouseOverWidget = nullptr;
		}

		if (mouseOverWidget != m_mouseOverWidget.lock().get())
		{
			handleMouseLeave();
			if (mouseOverWidget != nullptr)
			{
				m_mouseOverWidget = mouseOverWidget->getWeakPtr();
			}
			else
			{
				m_mouseOverWidget.reset();
			}
			handleMouseEnter();
		}

		handleMouseMove();
	}

	if (mouse->isJustPressed(M(LEFT)))
	{
		handleClick();
	}
}

void RootWidget::handleClick()
{
	bool eventHandled = false;
	std::weak_ptr<Widget> widget = m_mouseOverWidget;
	while (!widget.expired() && !eventHandled)
	{
		Widget* w = widget.lock().get();
		FLAT_ASSERT(w != nullptr);
		w->click(w, eventHandled);
		widget = w->getParent();
	}
}

void RootWidget::handleMouseMove()
{
	bool eventHandled = false;
	std::weak_ptr<Widget> widget = m_mouseOverWidget;
	while (!widget.expired() && !eventHandled)
	{
		Widget* w = widget.lock().get();
		FLAT_ASSERT(w != nullptr);
		w->mouseMove(w, eventHandled);
		widget = w->getParent();
	}
}

void RootWidget::handleMouseEnter()
{
	if (Widget* mouseOverWidget = m_mouseOverWidget.lock().get())
	{
		mouseOverWidget->m_mouseOver = true;
		mouseOverWidget->mouseEnter(mouseOverWidget);
	}
}

void RootWidget::handleMouseLeave()
{
	if (Widget* mouseOverWidget = m_mouseOverWidget.lock().get())
	{
		mouseOverWidget->m_mouseOver = false;
		mouseOverWidget->mouseLeave(mouseOverWidget);
	}
}

} // ui
} // sharp
} // flat
