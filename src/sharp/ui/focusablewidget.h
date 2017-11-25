#ifndef FLAT_SHARP_UI_FOCUSABLEWIDGET_H
#define FLAT_SHARP_UI_FOCUSABLEWIDGET_H

#include "widget.h"

namespace flat
{
namespace sharp
{
namespace ui
{

class FocusableWidget : public virtual Widget
{
	friend class RootWidget;
	public:
		FocusableWidget();
		FocusableWidget(const FocusableWidget&) = delete;
		FocusableWidget(FocusableWidget&&) = delete;
		~FocusableWidget();
		FocusableWidget& operator=(const FocusableWidget&) = delete;

		bool canBeFocused() const override;

		inline bool hasFocus() const { return m_hasFocus; }

	public:
		Slot<Widget*> enterFocus;
		Slot<Widget*> leaveFocus;

	private:
		bool m_hasFocus;
};

} // ui
} // sharp
} // flat

#endif // FLAT_SHARP_UI_FOCUSABLEWIDGET_H


