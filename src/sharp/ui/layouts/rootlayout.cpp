#include "rootlayout.h"
#include "../rootwidget.h"
#include "../../../video/window.h"
#include "../../../flat/game.h"

namespace flat
{
namespace sharp
{
namespace ui
{

void RootLayout::preLayout(Widget& widget)
{
	widget.to<RootWidget>().m_dirtyWidgets.clear();
}

void RootLayout::layout(Widget& widget, bool computePosition)
{
	Flat& flat = widget.to<RootWidget>().m_flat;
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
