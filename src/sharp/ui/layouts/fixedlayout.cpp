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
	childrenPreLayout(widget);
}

void FixedLayout::layout(Widget& widget)
{
	computeTransform(widget);
	childrenLayout(widget);
}

void FixedLayout::postLayout(Widget& widget)
{
	childrenPostLayout(widget);
}

} // ui
} // sharp
} // flat
