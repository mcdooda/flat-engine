#include "fixedlayout.h"
#include "../widget.h"

namespace flat
{
namespace sharp
{
namespace ui
{

void FixedLayout::preLayout(Widget& widget)
{
	Widget::SizePolicy sizePolicy = getSizePolicy(widget);

	if ((sizePolicy & Widget::SizePolicy::EXPAND_X) != 0)
	{
		computeExpandWidth(widget);
	}
	else
	{
		computeFixedWidth(widget);
	}

	if ((sizePolicy & Widget::SizePolicy::EXPAND_Y) != 0)
	{
		computeExpandHeight(widget);
	}
	else
	{
		computeFixedHeight(widget);
	}

	computeTransform(widget);
}

void FixedLayout::layout(Widget& widget, bool computePosition)
{
	childrenPreLayout(widget);
	childrenLayout(widget);
}

void FixedLayout::postLayout(Widget& widget)
{
	childrenPostLayout(widget);
}

} // ui
} // sharp
} // flat
