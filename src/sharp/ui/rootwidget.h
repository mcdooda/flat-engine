#ifndef FLAT_SHARP_UI_ROOTWIDGET_H
#define FLAT_SHARP_UI_ROOTWIDGET_H

#include "widget.h"
#include "layouts/rootlayout.h"

namespace flat
{
class Game;

namespace video
{
class Window;
}

namespace sharp
{
namespace ui
{

class RootWidget : public WidgetImpl<RootLayout>
{
	friend class RootLayout;

	typedef WidgetImpl<RootLayout> Super;
	public:
		RootWidget(Game& game);

		void draw(const flat::util::RenderSettings& renderSettings) const override final;

		void updateInput();
		void handleClick();
		
	private:
		Game& m_game;
		Widget* m_mouseOverWidget;
};

} // ui
} // sharp
} // flat

#endif // FLAT_SHARP_UI_WIDGET_H



