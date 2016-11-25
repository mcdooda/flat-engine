#ifndef FLAT_SHARP_UI_LAYOUTS_FIXEDLAYOUT_H
#define FLAT_SHARP_UI_LAYOUTS_FIXEDLAYOUT_H

#include "../layout.h"

namespace flat
{
namespace sharp
{
namespace ui
{

class FixedLayout : public Layout
{
public:
	static void preLayout(Widget& widget);
	static void layout(Widget& widget, bool computePosition);
	static void postLayout(Widget& widget);
};

} // ui
} // sharp
} // flat

#endif // FLAT_SHARP_UI_LAYOUTS_FIXEDLAYOUT_H



