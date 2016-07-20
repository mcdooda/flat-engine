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

class TextWidget : public WidgetImpl<FixedLayout>, public flat::video::font::String
{
	public:
		TextWidget(std::shared_ptr<const flat::video::font::Font> font);
		
		void setText(const std::string& text) override;
		
		void draw(const util::RenderSettings& renderSettings) const override;
};

} // ui
} // sharp
} // flat

#endif // FLAT_SHARP_UI_TEXTWIDGET_H



