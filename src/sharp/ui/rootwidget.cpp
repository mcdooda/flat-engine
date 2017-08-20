#include "rootwidget.h"
#include "focusablewidget.h"
#include "../../video/window.h"
#include "../../flat.h"

namespace flat
{
namespace sharp
{
namespace ui
{

RootWidget::RootWidget(Flat& flat) : Super(),
	m_flat(flat),
	m_dirty(true)
{
	setSizePolicy(Widget::SizePolicy::FIXED);
}

RootWidget::~RootWidget()
{
	// avoid calling leave focus as all the widgets are being destroyed
	if (!m_focusWidget.expired())
	{
		FocusableWidget* focusableWidget = dynamic_cast<FocusableWidget*>(m_focusWidget.lock().get());
		FLAT_ASSERT(focusableWidget != nullptr);
		focusableWidget->m_hasFocus = false;
	}
}

void RootWidget::clearAll()
{
	m_children.clear();
	m_mouseOverWidget.reset();
	m_focusWidget.reset();
	setDirty();
}

void RootWidget::draw(const flat::render::RenderSettings& renderSettings) const
{
	glEnable(GL_SCISSOR_TEST);
	ScissorRectangle screenScissor;
	getScissor(screenScissor);
	drawChildren(renderSettings, screenScissor);
	glDisable(GL_SCISSOR_TEST);
}

void RootWidget::addDirtyWidget(const std::weak_ptr<Widget>& widget)
{
	FLAT_ASSERT(!widget.expired());

	if (m_dirty)
		return;

	Widget* widgetPtr = widget.lock().get();
	std::vector<std::weak_ptr<Widget>>::iterator it = std::find_if(
		m_dirtyWidgets.begin(),
		m_dirtyWidgets.end(),
		[widgetPtr](std::weak_ptr<Widget>& w) { return w.lock().get() == widgetPtr; }
	);
	if (it == m_dirtyWidgets.end())
	{
		m_dirtyWidgets.push_back(widget);
	}
}

void RootWidget::removeDirtyWidget(const std::weak_ptr<Widget>& widget)
{
	FLAT_ASSERT(!widget.expired());

	if (m_dirty)
		return;

	Widget* widgetPtr = widget.lock().get();
	std::vector<std::weak_ptr<Widget>>::iterator it = std::find_if(
		m_dirtyWidgets.begin(),
		m_dirtyWidgets.end(),
		[widgetPtr](std::weak_ptr<Widget>& w) { return w.lock().get() == widgetPtr; }
	);
	if (it != m_dirtyWidgets.end())
	{
		m_dirtyWidgets.erase(it);
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
		for (std::weak_ptr<Widget>& widget : m_dirtyWidgets)
		{
			if (!widget.expired())
			{
				widget.lock()->fullLayout();
			}
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

#ifdef FLAT_DEBUG
void RootWidget::clearDirty()
{
	FLAT_ASSERT_MSG(false, "clearDirty on RootWidget");
}
#endif

void RootWidget::update(float dt)
{
	if (m_flat.input->window->isResized())
		fullLayout();

	bool updateMouseOver = updateDirtyWidgets();
	updateInput(updateMouseOver, dt);
}

void RootWidget::updateInput(bool updateMouseOver, float dt)
{
	auto& mouse = m_flat.input->mouse;

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

	if (mouse->wheelJustMoved())
	{
		handleMouseWheel(dt);
	}
}

void RootWidget::handleClick()
{
	{
		// propagate click until the event is handled
		bool eventHandled = false;
		Widget* widget = m_mouseOverWidget.lock().get();
		while (widget != nullptr && !eventHandled)
		{
			widget->click(widget, eventHandled);
			widget = widget->getParent().lock().get();
		}
	}

	{
		Widget* widget = m_mouseOverWidget.lock().get();
		while (widget != nullptr && !widget->canBeFocused())
		{
			widget = widget->getParent().lock().get();
		}
		
		Widget* previousFocusWidget = m_focusWidget.lock().get();
		Widget* newFocusWidget = widget != nullptr && widget->canBeFocused() ? widget : nullptr;

		FLAT_ASSERT(previousFocusWidget == nullptr || previousFocusWidget->canBeFocused());
		FLAT_ASSERT(newFocusWidget == nullptr || newFocusWidget->canBeFocused());

		if (previousFocusWidget != newFocusWidget)
		{
			if (previousFocusWidget != nullptr)
			{
				FocusableWidget* focusableWidget = dynamic_cast<FocusableWidget*>(previousFocusWidget);
				FLAT_ASSERT(focusableWidget != nullptr);
				focusableWidget->m_hasFocus = false;
				focusableWidget->leaveFocus(previousFocusWidget);
			}

			if (newFocusWidget != nullptr)
			{
				FocusableWidget* focusableWidget = dynamic_cast<FocusableWidget*>(newFocusWidget);
				FLAT_ASSERT(focusableWidget != nullptr);
				focusableWidget->m_hasFocus = true;
				focusableWidget->enterFocus(newFocusWidget);

				m_focusWidget = newFocusWidget->getWeakPtr();
			}
			else
			{
				m_focusWidget.reset();
			}

		}
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

void RootWidget::handleMouseWheel(float dt)
{
	auto& mouse = m_flat.input->mouse;
	const Vector2& wheelMove = mouse->getWheelMove();

	if (wheelMove.x != 0.f)
	{
		Widget* scrollableWidgetX = m_mouseOverWidget.lock().get();
		while (scrollableWidgetX != nullptr && !scrollableWidgetX->getAllowScrollX())
		{
			scrollableWidgetX = scrollableWidgetX->getParent().lock().get();
		}

		if (scrollableWidgetX != nullptr)
		{
			scrollableWidgetX->scrollX(wheelMove.x, dt);
		}
	}

	if (wheelMove.y != 0.f)
	{
		Widget* scrollableWidgetY = m_mouseOverWidget.lock().get();
		while (scrollableWidgetY != nullptr && !scrollableWidgetY->getAllowScrollY())
		{
			scrollableWidgetY = scrollableWidgetY->getParent().lock().get();
		}

		if (scrollableWidgetY != nullptr)
		{
			scrollableWidgetY->scrollY(wheelMove.y, dt);
		}
	}
}

} // ui
} // sharp
} // flat
