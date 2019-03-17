#include "sharp/ui/layouts/rootlayout.h"
#include "sharp/ui/rootwidget.h"

#include "flat/game.h"
#include "video/window.h"

namespace flat
{
namespace sharp
{
namespace ui
{

void RootLayout::preLayout(Widget& widget)
{
	dynamic_cast<RootWidget*>(&widget)->m_dirtyWidgets.clear();
}

void RootLayout::layout(Widget& widget, bool computePosition)
{
	Flat& flat = dynamic_cast<RootWidget*>(&widget)->m_flat;
	flat::video::Window* window = flat.video->window;
	widget.setSize(window->getSize());
	getComputedSize(widget) = getSize(widget);

	getTransform(widget) = Matrix4();
}

void RootLayout::postLayout(Widget& widget)
{
	childrenFullLayout(widget);
}

} // ui
} // sharp
} // flat
