#ifndef FLAT_SHARP_UI_ROOTWIDGET_H
#define FLAT_SHARP_UI_ROOTWIDGET_H

#include "sharp/ui/widget.h"
#include "sharp/ui/layouts/rootlayout.h"

namespace flat
{
class Flat;

namespace video
{
class Window;
}

namespace sharp
{
namespace ui
{

class RootWidget : public WidgetImpl<RootLayout>
{
	friend class RootLayout;

	using Super = WidgetImpl<RootLayout>;
	public:
		RootWidget() = delete;
		RootWidget(const RootWidget&) = delete;
		RootWidget(RootWidget&&) = delete;
		RootWidget(Flat& flat);
		~RootWidget();
		RootWidget& operator=(const RootWidget&) = delete;

		void clearAll();

		void draw(const flat::render::RenderSettings& renderSettings) const; // not an override

		void addDirtyWidget(const std::weak_ptr<Widget>& widget);
		void removeDirtyWidget(const std::weak_ptr<Widget>& widget);
		void setDirty() override;
		FLAT_DEBUG_ONLY(void clearDirty() override;)

		void update();

		inline bool isMouseOver() const { return !m_mouseOverWidget.expired(); }
		inline const std::weak_ptr<Widget>& getCurrentMouseOverWidget() const { return m_mouseOverWidget; }

		bool isRoot() const override { return true; }

		void drag(Widget* widget);
		void drop(Widget* widget);

		void focus(Widget* widget);

		CursorType getCursorType() const override;

	private:
		bool updateDirtyWidgets();
		void updateDraggedWidgets();
		void updateDragScrollingWidget();
		void updateInput(bool updateMouseOver);
		void updateCursor() const;

	private:
		void handleLeftMouseButtonDown();
		void handleLeftMouseButtonUp();
		void handleRightMouseButtonDown();
		void handleRightMouseButtonUp();
		void handleMouseMove(bool movedOverCurrentWidget);
		void handleMouseEnter(Widget* previousMouseOverWidget);
		void handleMouseLeave(Widget* nextMouseOverWidget);
		void handleMouseWheel();
		void handleTabButtonPressed();
		void handleCopy();
		void handlePaste();

		Widget* getFocusableChildren(Widget* widget);
		Widget* getNextFocusable(Widget* widget);

		bool focusChildren(Widget* widget);
		void focusNext(Widget* widget);

		template <class... Args>
		static bool propagateEvent(Widget* widget, Slot<Widget*, bool&, Args...> Widget::* slot, Args... args);
		
	private:
		Flat& m_flat;
		std::weak_ptr<Widget> m_mouseOverWidget;
		std::weak_ptr<Widget> m_mouseDownWidget;
		std::weak_ptr<Widget> m_focusWidget;

		std::vector<std::tuple<std::weak_ptr<Widget>, Vector2>> m_draggedWidgets;

		std::weak_ptr<Widget> m_dragScrollingWidget;
		Vector2 m_dragScrollingOffset;

		std::vector<std::weak_ptr<Widget>> m_dirtyWidgets;
		bool m_dirty : 1;
		bool m_dragScrolled : 1;
};

template <class... Args>
bool RootWidget::propagateEvent(Widget* widget, Slot<Widget*, bool&, Args...> Widget::* slot, Args... args)
{
	// propagate the event upwards until it is handled
	bool eventHandled = false;
	while (widget != nullptr && !eventHandled)
	{
		(widget->*slot)(widget, eventHandled, args...);
		widget = widget->getParent().lock().get();
	}
	return eventHandled;
}
} // ui
} // sharp
} // flat

#endif // FLAT_SHARP_UI_ROOTWIDGET_H



