#ifndef FLAT_SHARP_UI_ROOTLAYOUT_H
#define FLAT_SHARP_UI_ROOTLAYOUT_H

#include "../layout.h"

namespace flat
{
namespace sharp
{
namespace ui
{
class RootWidget;

class RootLayout : public Layout
{
public:
	static void preLayout(Widget& widget);
	static void layout(Widget& widget);
	static void postLayout(Widget& widget);
};

} // ui
} // sharp
} // flat

#endif // FLAT_SHARP_UI_WIDGET_H



