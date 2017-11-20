#ifndef FLAT_SHARP_UI_TEXTINPUTWIDGET_H
#define FLAT_SHARP_UI_TEXTINPUTWIDGET_H

#include "focusablewidget.h"
#include "textwidget.h"

#include "../../input/context/inputcontext.h"

#pragma warning(disable: 4250)

namespace flat
{
namespace sharp
{
namespace ui
{
class Widget;

class TextInputWidget : public FocusableWidget, public TextWidget
{
	using Widget = ui::Widget;
	public:
		TextInputWidget() = delete;
		TextInputWidget(const TextInputWidget&) = delete;
		TextInputWidget(TextInputWidget&&) = delete;
		TextInputWidget(Flat& flat, const std::shared_ptr<const video::font::Font>& font);
		virtual ~TextInputWidget();
		TextInputWidget& operator=(const TextInputWidget&) = delete;

	public:
		Slot<Widget*> valueChanged;
		Slot<Widget*> submit;
	
	private:
		bool enteredFocus(Widget* widget);
		bool leftFocus(Widget* widget);
		virtual bool keyJustPressed(input::Key key);
		virtual bool textEdited(const std::string& text);

	private:
		Flat& m_flat;
		std::shared_ptr<input::context::InputContext> m_inputContext;
};

} // ui
} // sharp
} // flat

#endif // FLAT_SHARP_UI_TEXTINPUTWIDGET_H


