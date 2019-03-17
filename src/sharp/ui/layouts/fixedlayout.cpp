#include "sharp/ui/layouts/fixedlayout.h"
#include "sharp/ui/widget.h"

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
	else if ((sizePolicy & Widget::SizePolicy::COMPRESS_X) != 0)
	{
		computeCompressWidth(widget);
	}
	else
	{
		FLAT_ASSERT((sizePolicy & Widget::SizePolicy::FIXED_X) != 0);
		computeFixedWidth(widget);
	}

	if ((sizePolicy & Widget::SizePolicy::EXPAND_Y) != 0)
	{
		computeExpandHeight(widget);
	}
	else if ((sizePolicy & Widget::SizePolicy::COMPRESS_Y) != 0)
	{
		computeCompressHeight(widget);
	}
	else
	{
		FLAT_ASSERT((sizePolicy & Widget::SizePolicy::FIXED_Y) != 0);
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
