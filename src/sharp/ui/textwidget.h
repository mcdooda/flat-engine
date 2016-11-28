#ifndef FLAT_SHARP_UI_TEXTWIDGET_H
#define FLAT_SHARP_UI_TEXTWIDGET_H

#include "widget.h"
#include "layouts/fixedlayout.h"
#include "../../video/font/string.h"

namespace flat
{
namespace sharp
{
namespace ui
{

class TextWidget : public WidgetImpl<FixedLayout>, public video::font::String
{
	typedef WidgetImpl<FixedLayout> Widget;
	public:
		TextWidget(const std::shared_ptr<const video::font::Font>& font);
		
		void setText(const std::string& text);
		inline void setTextColor(const video::Color& textColor) { m_textColor = textColor; }
		
		void draw(const render::RenderSettings& renderSettings) const override;

	private:
		video::Color m_textColor;
};

} // ui
} // sharp
} // flat

#endif // FLAT_SHARP_UI_TEXTWIDGET_H



