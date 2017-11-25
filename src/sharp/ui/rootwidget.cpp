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
	m_dirty(true),
	m_dragScrolled(false)
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
	m_mouseDownWidget.reset();
	m_focusWidget.reset();
	m_draggedWidgets.clear();
	m_mouseOver = false;
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
	else if (!m_dirtyWidgets.empty())
	{
		for (std::weak_ptr<Widget>& widget : m_dirtyWidgets)
		{
			if (!widget.expired())
			{
				widget.lock()->fullLayout();
			}
		}
		m_dirtyWidgets.clear();
		return true;
	}
	return false;
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

void RootWidget::update()
{
	if (m_flat.input->window->isResized())
	{
		fullLayout();
	}

	updateDraggedWidgets();
	updateDragScrollingWidget();

	bool updateMouseOver = updateDirtyWidgets();
	updateInput(updateMouseOver);
}

void RootWidget::updateDraggedWidgets()
{
	auto& mouse = m_flat.input->mouse;
	for (const std::tuple<std::weak_ptr<Widget>, Vector2>& draggedWidget : m_draggedWidgets)
	{
		Widget* widget = std::get<0>(draggedWidget).lock().get();
		const Vector2& relativePosition = std::get<1>(draggedWidget);
		widget->setAbsolutePosition(mouse->getPosition() - relativePosition);
		widget->m_dragged = true;
	}
}

void RootWidget::updateDragScrollingWidget()
{
	if (!m_dragScrollingWidget.expired())
	{
		Widget* widget = m_dragScrollingWidget.lock().get();
		FLAT_ASSERT(widget != nullptr);

		auto& mouse = m_flat.input->mouse;
		ScrollPosition newScrollPosition = -m_dragScrollingOffset - mouse->getPosition();
		if (newScrollPosition != widget->getScrollPosition())
		{
			widget->setScrollPosition(newScrollPosition);
			m_dragScrolled = true;
		}
	}
}

void RootWidget::updateInput(bool updateMouseOver)
{
	auto& mouse = m_flat.input->mouse;

	if (updateMouseOver || mouse->justMoved())
	{
		Widget* mouseOverWidget = getMouseOverWidget(mouse->getPosition());
		if (mouseOverWidget == this) // root
		{
			mouseOverWidget = nullptr;
			m_flat.ui->setDefaultCursor();
		}

		if (mouseOverWidget != m_mouseOverWidget.lock().get())
		{
			handleMouseLeave();
			if (mouseOverWidget != nullptr)
			{
				m_flat.ui->setCursor(mouseOverWidget->getCursorType());
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
		handleLeftMouseButtonDown();
	}
	else if (mouse->isJustReleased(M(LEFT)))
	{
		handleLeftMouseButtonUp();
	}

	if (mouse->isJustPressed(M(RIGHT)))
	{
		handleRightMouseButtonDown();
	}
	else if (mouse->isJustReleased(M(RIGHT)))
	{
		handleRightMouseButtonUp();
	}

	if (mouse->wheelJustMoved())
	{
		handleMouseWheel();
	}
}

void RootWidget::drag(Widget* widget)
{
	FLAT_ASSERT(widget != nullptr);
	auto& mouse = m_flat.input->mouse;
	Vector2 relativePosition = widget->getRelativePosition(mouse->getPosition());
	m_draggedWidgets.emplace_back(widget->getWeakPtr(), relativePosition);
}

void RootWidget::drop(Widget* widget)
{
	FLAT_ASSERT(widget != nullptr);
	std::vector<std::tuple<std::weak_ptr<Widget>, Vector2>>::iterator it = std::find_if(
		m_draggedWidgets.begin(),
		m_draggedWidgets.end(),
		[widget](const std::tuple<std::weak_ptr<Widget>, Vector2>& draggedWidget)
		{
			return std::get<0>(draggedWidget).lock().get() == widget;
		}
	);
	FLAT_ASSERT(it != m_draggedWidgets.end());
	m_draggedWidgets.erase(it);
}

void RootWidget::focus(Widget* widget)
{
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

void RootWidget::handleLeftMouseButtonDown()
{
	m_mouseDownWidget = m_mouseOverWidget;

	propagateEvent(m_mouseOverWidget.lock().get(), &Widget::leftClick);
	propagateEvent(m_mouseDownWidget.lock().get(), &Widget::mouseDown);

	// find a focusable widget if possible
	{
		Widget* widget = m_mouseOverWidget.lock().get();
		while (widget != nullptr && !widget->canBeFocused())
		{
			widget = widget->getParent().lock().get();
		}

		focus(widget);
	}
}

void RootWidget::handleLeftMouseButtonUp()
{
	Widget* mouseDownWidget = m_mouseDownWidget.lock().get();
	Widget* mouseOverWidget = m_mouseOverWidget.lock().get();
	bool eventHandled = false;
	if (mouseDownWidget != mouseOverWidget)
	{
		eventHandled = propagateEvent(mouseOverWidget, &Widget::mouseUp);
	}
	if (!eventHandled)
	{
		propagateEvent(mouseDownWidget, &Widget::mouseUp);
	}
}

void RootWidget::handleRightMouseButtonDown()
{
	m_dragScrolled = false;

	Widget* mouseOverWidget = m_mouseOverWidget.lock().get();

	Widget* widget = mouseOverWidget;
	while (widget != nullptr && !widget->getAllowDragScrolling())
	{
		widget = widget->getParent().lock().get();
	}

	if (widget != nullptr)
	{
		auto& mouse = m_flat.input->mouse;

		m_dragScrollingWidget = widget->getWeakPtr();
		m_dragScrollingOffset = -widget->getScrollPosition() - mouse->getPosition();
	}
	else if (mouseOverWidget != nullptr)
	{
		propagateEvent(mouseOverWidget, &Widget::rightClick);
	}
}

void RootWidget::handleRightMouseButtonUp()
{
	Widget* mouseOverWidget = m_mouseOverWidget.lock().get();
	if (mouseOverWidget != nullptr)
	{
		m_dragScrollingWidget.reset();
	}
	
	if (!m_dragScrolled)
	{
		propagateEvent(mouseOverWidget, &Widget::rightClick);
	}
	m_dragScrolled = false;
}

void RootWidget::handleMouseMove()
{
	propagateEvent(m_mouseOverWidget.lock().get(), &Widget::mouseMove);
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

void RootWidget::handleMouseWheel()
{
	auto& mouse = m_flat.input->mouse;
	const Vector2& wheelMove = mouse->getWheelMove();

	if (wheelMove.x != 0.f || wheelMove.y != 0.f)
	{
		propagateEvent<const Vector2&>(m_mouseOverWidget.lock().get(), &Widget::mouseWheelMove, wheelMove);
	}
}

} // ui
} // sharp
} // flat
