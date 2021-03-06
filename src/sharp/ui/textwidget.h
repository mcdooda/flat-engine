#ifndef FLAT_SHARP_UI_TEXTWIDGET_H
#define FLAT_SHARP_UI_TEXTWIDGET_H

#include "sharp/ui/widget.h"
#include "sharp/ui/layouts/fixedlayout.h"

#include "video/font/string.h"

namespace flat
{
namespace sharp
{
namespace ui
{

class TextWidget : public virtual WidgetImpl<FixedLayout>, public video::font::String
{
	using Widget = WidgetImpl<FixedLayout>;
	public:
		TextWidget() = delete;
		TextWidget(const TextWidget&) = delete;
		TextWidget(TextWidget&&) = delete;
		TextWidget(const std::shared_ptr<const video::font::Font>& font);
		~TextWidget() = default;

		void setText(const std::string& text);
		void setTextColor(const video::Color& textColor);
		inline const video::Color& getTextColor() const { return m_textColor; }

		void draw(const render::RenderSettings& renderSettings, const ScissorRectangle& parentScissor) const override;

	private:
		video::Color m_textColor;
};

} // ui
} // sharp
} // flat

#endif // FLAT_SHARP_UI_TEXTWIDGET_H



