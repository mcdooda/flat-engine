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
	computeFixedHeight(widget);
	computeFixedWidth(widget);
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
