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
	m_inputContext->getKeyboardInputContext().setEnableKeyRepeat(true);
	m_flat.input->pushContext(m_inputContext);
	m_inputContext->getKeyboardInputContext().keyJustPressed.on(this, &TextInputWidget::keyJustPressed);
	m_inputContext->getKeyboardInputContext().textEdited.on(this, &TextInputWidget::textEdited);
	m_inputContext->getKeyboardInputContext().setEnableTextInput(true);
	return true;
}

bool TextInputWidget::leftFocus(Widget* widget)
{
	m_inputContext->getKeyboardInputContext().keyJustPressed.off(this);
	m_inputContext->getKeyboardInputContext().textEdited.off(this);
	m_inputContext->getKeyboardInputContext().setEnableTextInput(false);
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
	if (text != getText())
	{
		setText(text);
		valueChanged(this);
	}
	else if (key == K(RETURN))
	{
		submit(this);
	}
	return true;
}

bool TextInputWidget::textEdited(const std::string& text)
{
	std::string currentText = getText();
	currentText += text;
	if (currentText != getText())
	{
		setText(currentText);
		valueChanged(this);
	}
	return true;
}

} // ui
} // sharp
} // flat


