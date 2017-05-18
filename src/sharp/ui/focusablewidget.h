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
	public:
		FocusableWidget() = default;
		FocusableWidget(const FocusableWidget&) = delete;
		FocusableWidget(FocusableWidget&&) = delete;
		~FocusableWidget() = default;
		FocusableWidget& operator=(const FocusableWidget&) = delete;

		bool canBeFocused() const override;

	public:
		Slot<Widget*> enterFocus;
		Slot<Widget*> leaveFocus;
};

} // ui
} // sharp
} // flat

#endif // FLAT_SHARP_UI_FOCUSABLEWIDGET_H


