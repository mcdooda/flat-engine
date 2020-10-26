#include "sharp/ui/rootwidget.h"

#include "flat.h"
#include "video/window.h"

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

	// initialize the root widget's size as soon as possible so it does not return 0 on first frame
	flat::video::Window* window = flat.video->window;
	setSize(window->getSize());
	m_computedSize = getSize();
	m_transform = Matrix4();
}

RootWidget::~RootWidget()
{
	// avoid calling leave focus as all the widgets are being destroyed
	if (!m_focusWidget.expired())
	{
		Widget* focusableWidget = m_focusWidget.lock().get();
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
	setDirty();
}

void RootWidget::draw(const flat::render::RenderSettings& renderSettings) const
{
	glEnable(GL_SCISSOR_TEST);
	ScissorRectangle screenScissor;
	getScissor(screenScissor);
	glEnableVertexAttribArray(renderSettings.positionAttribute);
	drawChildren(renderSettings, screenScissor);
	glDisableVertexAttribArray(renderSettings.positionAttribute);
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
		fullLayout();
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

	// call twice to mitigate one frame late widgets like tooltips, costs nothing if everything is up to date
	bool updateMouseOver = updateDirtyWidgets();
	updateMouseOver = updateDirtyWidgets() || updateMouseOver;

	updateInput(updateMouseOver);

	updateCursor();
}

void RootWidget::updateDraggedWidgets()
{
	auto& mouse = m_flat.input->mouse;
	for (const std::tuple<std::weak_ptr<Widget>, Vector2>& draggedWidget : m_draggedWidgets)
	{
		Widget* widget = std::get<0>(draggedWidget).lock().get();
		const Vector2& relativePosition = std::get<1>(draggedWidget);
		Vector2 absolutePosition = mouse->getPosition() - relativePosition;
		widget->beforeDrag(widget, absolutePosition);
		widget->setAbsolutePosition(absolutePosition);
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
			m_flat.ui->setCursorOverride(CURSOR(SIZEALL));
			widget->setScrollPosition(newScrollPosition);
			m_dragScrolled = true;
		}
	}
}

void RootWidget::updateInput(bool updateMouseOver)
{
	auto& mouse = m_flat.input->mouse;
	auto& keyboard = m_flat.input->keyboard;
	if (keyboard->isJustPressed(K(TAB)))
	{
		handleTabButtonPressed(keyboard->isPressed(K(LSHIFT)));
	}
	else if (keyboard->isJustPressed(K(C)) && (keyboard->isPressed(K(LCTRL)) || keyboard->isPressed(K(RCTRL))))
	{
		handleCopy();
	}
	else if (keyboard->isJustPressed(K(V)) && (keyboard->isPressed(K(LCTRL)) || keyboard->isPressed(K(RCTRL))))
	{
		handlePaste();
	}
	else if (keyboard->isJustPressed(K(Z)) && (keyboard->isPressed(K(LSHIFT)) || keyboard->isPressed(K(RSHIFT)))
		&& (keyboard->isPressed(K(LCTRL)) || keyboard->isPressed(K(RCTRL))))
	{
		handleRedo();
	}
	else if (keyboard->isJustPressed(K(Z)) && (keyboard->isPressed(K(LCTRL)) || keyboard->isPressed(K(RCTRL))))
	{
		handleUndo();
	}
	else if (keyboard->isJustPressed(K(Y)) && (keyboard->isPressed(K(LCTRL)) || keyboard->isPressed(K(RCTRL))))
	{
		handleRedo();
	}

	const bool mouseMoved = mouse->justMoved();
	if (updateMouseOver || mouseMoved)
	{
		Widget* mouseOverWidget = getMouseOverWidget(mouse->getPosition());
		if (mouseOverWidget == this) // root
		{
			mouseOverWidget = nullptr;
		}

		Widget* previousMouseOverWidget = m_mouseOverWidget.lock().get();
		const bool mouseOverWidgetChanged = mouseOverWidget != previousMouseOverWidget;
		if (mouseOverWidgetChanged)
		{
			handleMouseLeave(mouseOverWidget);
			if (mouseOverWidget != nullptr)
			{
				m_mouseOverWidget = mouseOverWidget->getWeakPtr();
			}
			else
			{
				m_mouseOverWidget.reset();
			}
			handleMouseEnter(previousMouseOverWidget);
		}

		handleMouseMove(mouseOverWidgetChanged || mouseMoved);
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

	handleGamepadButtonPressed();
	handleGamepadButtonReleased();
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
	if (it != m_draggedWidgets.end())
	{
		m_draggedWidgets.erase(it);
	}
}

void RootWidget::focus(Widget* widget)
{
	Widget* previouslyFocusedWidget = m_focusWidget.lock().get();
	Widget* newlyFocusedWidget = widget != nullptr && widget->isFocusable() ? widget : nullptr;

	FLAT_ASSERT(previouslyFocusedWidget == nullptr || previouslyFocusedWidget->isFocusable());
	FLAT_ASSERT(newlyFocusedWidget == nullptr || newlyFocusedWidget->isFocusable());

	if (previouslyFocusedWidget != newlyFocusedWidget)
	{
		if (previouslyFocusedWidget != nullptr)
		{
			previouslyFocusedWidget->m_hasFocus = false;
			previouslyFocusedWidget->leaveFocus(previouslyFocusedWidget);
		}

		if (newlyFocusedWidget != nullptr)
		{
			newlyFocusedWidget->m_hasFocus = true;
			newlyFocusedWidget->enterFocus(newlyFocusedWidget);

			m_focusWidget = newlyFocusedWidget->getWeakPtr();
		}
		else
		{
			m_focusWidget.reset();
		}
	}
}

void RootWidget::handleGamepadButtonPressed()
{
	const auto& gamepads = m_flat.input->gamepads;
	gamepads->triggerPressedButtons(0, [this](SDL_GameControllerButton button){
		triggerGlobalEvent(this, &Widget::gamepadButtonDown, 0, button);
	});
}

void RootWidget::handleGamepadButtonReleased()
{
	const auto& gamepads = m_flat.input->gamepads;
	gamepads->triggerReleasedButtons(0, [this](SDL_GameControllerButton button){
		triggerGlobalEvent(this, &Widget::gamepadButtonUp, 0, button);
	});
}

void RootWidget::handleLeftMouseButtonDown()
{
	m_mouseDownWidget = m_mouseOverWidget;

	propagateEvent(m_mouseOverWidget.lock().get(), &Widget::leftClick);
	propagateEvent(m_mouseDownWidget.lock().get(), &Widget::mouseDown);

	// find a focusable widget if possible
	{
		Widget* widget = m_mouseOverWidget.lock().get();
		while (widget != nullptr && !widget->isFocusable())
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

	m_mouseDownWidget.reset();
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
	Widget* mouseOverWidget = m_dragScrollingWidget.lock().get();
	if (mouseOverWidget != nullptr)
	{
		m_dragScrollingWidget.reset();
		m_flat.ui->resetCursorOverride();
	}
	if (!m_dragScrolled)
	{
		propagateEvent(m_mouseOverWidget.lock().get(), &Widget::rightClick);
	}
	m_dragScrolled = false;
}

void RootWidget::handleMouseMove(bool movedOverCurrentWidget)
{
	Widget* mouseDownWidget = m_mouseDownWidget.lock().get();
	Widget* mouseOverWidget = m_mouseOverWidget.lock().get();

	if (movedOverCurrentWidget)
	{
		propagateEvent(mouseDownWidget, &Widget::mouseMove);
	}

	if (mouseOverWidget != mouseDownWidget)
	{
		propagateEvent(mouseOverWidget, &Widget::mouseMove);
	}
}

void RootWidget::handleMouseEnter(Widget* previousMouseOverWidget)
{
	Widget* widget = m_mouseOverWidget.lock().get();
	if (widget == nullptr)
	{
		return;
	}

	Widget* commonAncestor = nullptr;
	if (previousMouseOverWidget == nullptr)
	{
		commonAncestor = this;
	}
	else
	{
		commonAncestor = getCommonAncestor(widget, previousMouseOverWidget);
	}

	while (widget != commonAncestor)
	{
		widget->mouseEnter(widget);
		widget = widget->getParent().lock().get();
	}
}

void RootWidget::handleMouseLeave(Widget* nextMouseOverWidget)
{
	Widget* widget = m_mouseOverWidget.lock().get();
	if (widget == nullptr)
	{
		return;
	}
	
	Widget* commonAncestor = nullptr;
	if (nextMouseOverWidget == nullptr)
	{
		commonAncestor = this;
	}
	else
	{
		commonAncestor = getCommonAncestor(widget, nextMouseOverWidget);
	}

	while (widget != nullptr && widget != commonAncestor)
	{
		widget->mouseLeave(widget);
		widget = widget->getParent().lock().get();
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

std::vector<Widget*> getFocusableChildren(Widget* widget)
{
	std::vector<Widget*> result;
	if (widget->isFocusable())
	{
		result.push_back(widget);
	}
	const std::vector<std::shared_ptr<Widget>>& children = widget->getChildren();
	for(const std::shared_ptr<Widget>& child : children)
	{
		const std::vector<Widget*>& focusableChildren = getFocusableChildren(child.get());
		result.insert(result.end(), focusableChildren.begin(), focusableChildren.end());
	}
	return result;
}

template<class Iterator>
Widget* getClosestFocusable(Widget* widget, Iterator begin, Iterator end)
{
	auto iter = std::find_if(begin, end, [widget](Widget* a) -> bool
	{
		return a == widget;
	});

	if (++iter < end)
	{
		return *iter;
	}
	if (begin != end)
	{
		return *begin;
	}
	return nullptr;
}

Widget* RootWidget::getNextFocusable(Widget* widget)
{
	std::vector<Widget*> focusables = getFocusableChildren(this);
	return getClosestFocusable(widget, focusables.begin(), focusables.end());
}

Widget* RootWidget::getPreviousFocusable(Widget* widget)
{
	std::vector<Widget*> focusables = getFocusableChildren(this);
	return getClosestFocusable(widget, focusables.rbegin(), focusables.rend());
}

void RootWidget::handleTabButtonPressed(bool shiftPressed)
{
	Widget* focused = m_focusWidget.lock().get();
	if (focused != nullptr)
	{
		Widget* next = nullptr;

		if (shiftPressed)
		{
			next = getPreviousFocusable(focused);
		}
		else
		{
			next = getNextFocusable(focused);
		}

		if (next != nullptr)
		{
			focus(next);
		}
	}
}

void RootWidget::handleCopy()
{
	Widget* focused = m_focusWidget.lock().get();
	if (focused != nullptr)
	{
		std::string copied;
		focused->copy(focused, copied);
		SDL_SetClipboardText(copied.c_str());
	}
}

void RootWidget::handleUndo()
{
	Widget* focused = m_focusWidget.lock().get();
	if (focused != nullptr)
	{
		focused->undo(focused);
	}
}

void RootWidget::handleRedo()
{
	Widget* focused = m_focusWidget.lock().get();
	if (focused != nullptr)
	{
		focused->redo(focused);
	}
}

void RootWidget::handlePaste()
{
	Widget* focused = m_focusWidget.lock().get();
	if (focused != nullptr)
	{
		focused->paste(focused, SDL_GetClipboardText());
	}
}

CursorType RootWidget::getCursorType() const
{
	return CURSOR(ARROW);
}

void RootWidget::updateCursor() const
{
	Widget* cursorWidget = nullptr;

	if (!m_mouseDownWidget.expired())
	{
		cursorWidget = m_mouseDownWidget.lock().get();
	}
	else
	{
		cursorWidget = m_mouseOverWidget.lock().get();
	}

	if (cursorWidget != nullptr && cursorWidget->getRootIfAncestor() != nullptr)
	{
		m_flat.ui->setCursor(cursorWidget->getCursorType());
	}
	else
	{
		m_flat.ui->setDefaultCursor();
	}
}

} // ui
} // sharp
} // flat
