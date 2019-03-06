#include "sharp/ui/focusablewidget.h"

namespace flat
{
namespace sharp
{
namespace ui
{

FocusableWidget::FocusableWidget() :
	m_hasFocus(false)
{

}

FocusableWidget::~FocusableWidget()
{
	if (m_hasFocus)
	{
		m_hasFocus = false;
		leaveFocus(this);
	}
}

bool FocusableWidget::canBeFocused() const
{
	return true;
}

} // ui
} // sharp
} // flat


