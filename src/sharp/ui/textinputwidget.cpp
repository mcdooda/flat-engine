#include <cctype>

#include "textinputwidget.h"

#include "../../flat.h"

namespace flat
{
namespace sharp
{
namespace ui
{

TextInputWidget::TextInputWidget(Flat& flat, const std::shared_ptr<const video::font::Font>& font) : TextWidget(font),
	m_flat(flat)
{
	enterFocus.on(this, &TextInputWidget::enteredFocus);
	leaveFocus.on(this, &TextInputWidget::leftFocus);
}

TextInputWidget::~TextInputWidget()
{
	enterFocus.off(this);
	leaveFocus.off(this);
}

bool TextInputWidget::enteredFocus(Widget* widget)
{
	m_inputContext = std::make_unique<input::context::InputContext>(m_flat);
	m_flat.input->pushContext(m_inputContext);
	m_inputContext->getKeyboardInputContext().keyJustPressed.on(this, &TextInputWidget::keyJustPressed);
	return true;
}

bool TextInputWidget::leftFocus(Widget* widget)
{
	m_inputContext->getKeyboardInputContext().keyJustPressed.off(this);
	m_flat.input->popContext(m_inputContext);
	m_inputContext.reset();
	return true;
}

bool TextInputWidget::keyJustPressed(input::Key key)
{
	std::string text = getText();

	if (key == K(BACKSPACE))
	{
		if (!text.empty())
		{
			text = text.substr(0, text.size() - 1);
		}
	}
	else
	{
		char c = SDL_GetKeyFromScancode(key);
		const video::font::Font* font = getFont().get();
		FLAT_ASSERT(font != nullptr);
		if (font->isValidChar(c))
		{
			const input::context::KeyboardInputContext& keyboardInputContext = m_inputContext->getKeyboardInputContext();
			bool shiftPressed = keyboardInputContext.isPressed(K(LSHIFT)) || keyboardInputContext.isPressed(K(RSHIFT));
			if (std::islower(c) && shiftPressed)
			{
				c = std::toupper(c);
			}
			text += c;
		}
	}

	setText(text);
	return true;
}

} // ui
} // sharp
} // flat


