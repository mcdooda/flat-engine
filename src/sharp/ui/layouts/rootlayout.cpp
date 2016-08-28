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
	static_cast<RootWidget&>(widget).m_dirtyWidgets.clear();
}

void RootLayout::layout(Widget& widget)
{
	flat::video::Window* window = static_cast<RootWidget&>(widget).m_game.video->window;
	widget.setSize(window->getSize());

	getTransform(widget).setIdentity();
}

void RootLayout::postLayout(Widget& widget)
{
	childrenFullLayout(widget);
}

} // ui
} // sharp
} // flat
