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

		void draw(const flat::render::RenderSettings& renderSettings) const override; // final

		void addDirtyWidget(Widget* widget);
		void updateDirtyWidgets();
		void setDirty() override;

		void updateInput();

		inline bool isMouseOver() const { return m_mouseOverWidget != nullptr; }

		bool isRoot() const override { return true; }

	private:
		void handleClick();
		void handleMouseMove();
		void handleMouseEnter();
		void handleMouseLeave();
		
	private:
		Game& m_game;
		Widget* m_mouseOverWidget;
		std::vector<Widget*> m_dirtyWidgets;
		bool m_dirty : 1;
};

} // ui
} // sharp
} // flat

#endif // FLAT_SHARP_UI_ROOTWIDGET_H



