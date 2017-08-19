#ifndef FLAT_SHARP_UI_ROOTWIDGET_H
#define FLAT_SHARP_UI_ROOTWIDGET_H

#include "widget.h"
#include "layouts/rootlayout.h"

namespace flat
{
class Flat;

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

	using Super = WidgetImpl<RootLayout>;
	public:
		RootWidget() = delete;
		RootWidget(const RootWidget&) = delete;
		RootWidget(RootWidget&&) = delete;
		RootWidget(Flat& flat);
		~RootWidget();
		RootWidget& operator=(const RootWidget&) = delete;

		void clearAll();

		void draw(const flat::render::RenderSettings& renderSettings) const; // not an override

		void addDirtyWidget(const std::weak_ptr<Widget>& widget);
		void removeDirtyWidget(const std::weak_ptr<Widget>& widget);
		void setDirty() override;
		FLAT_DEBUG_ONLY(void clearDirty() override;)

		void update();

		inline bool isMouseOver() const { return !m_mouseOverWidget.expired(); }
		inline const std::weak_ptr<Widget>& getCurrentMouseOverWidget() const { return m_mouseOverWidget; }

		bool isRoot() const override { return true; }

	private:
		bool updateDirtyWidgets();
		void updateInput(bool updateMouseOver);

	private:
		void handleClick();
		void handleMouseMove();
		void handleMouseEnter();
		void handleMouseLeave();
		void handleMouseWheel();
		
	private:
		Flat& m_flat;
		std::weak_ptr<Widget> m_mouseOverWidget;
		std::weak_ptr<Widget> m_focusWidget;
		std::vector<std::weak_ptr<Widget>> m_dirtyWidgets;
		bool m_dirty : 1;
};

} // ui
} // sharp
} // flat

#endif // FLAT_SHARP_UI_ROOTWIDGET_H



