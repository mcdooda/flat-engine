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

void RootLayout::preLayout(Widget& /*widget*/)
{

}

void RootLayout::layout(Widget& widget)
{
	flat::video::Window* window = static_cast<RootWidget&>(widget).m_game.video->window;
	widget.setSize(window->getSize());

	getTransform(widget).setIdentity();
	// inverse y axis (top = 0, bottom = window height)
	//getTransform(widget).translate(0.f, -window->getSize().y);
	//getTransform(widget).scale(1.f, -1.f);
}

void RootLayout::postLayout(Widget& widget)
{
	childrenFullLayout(widget);
}

} // ui
} // sharp
} // flat
