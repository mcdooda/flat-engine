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
	m_flat(flat),
	m_cursor(0)
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
	m_cursor = getText().size();
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
	if (key == K(BACKSPACE) && !text.empty())
	{
		FLAT_ASSERT(m_cursor <= text.size());
		if (m_cursor > 0)
		{
			text = text.erase(m_cursor - 1, 1);
			if (text != getText())
			{
				moveCursor(-1);
				setText(text);
				valueChanged(this);
			}
		}
	}
	else if (key == K(RETURN) || key == K(KP_ENTER))
	{
		submit(this);
	}
	else if (key == K(LEFT))
	{
		moveCursor(-1);
	}
	else if (key == K(RIGHT))
	{
		moveCursor(1);
	}
	return true;
}

bool TextInputWidget::textEdited(const std::string& text)
{
	std::string currentText = getText();
	currentText.insert(m_cursor, text);
	if (currentText != getText())
	{
		setText(currentText);
		moveCursor(static_cast<int>(text.size()));
		valueChanged(this);
	}
	return true;
}

void TextInputWidget::moveCursor(int offset)
{
	if(offset > 0 || m_cursor >= static_cast<size_t>(std::abs(offset)))
	{
		m_cursor += offset;
		if (m_cursor < 0)
		{
			m_cursor = 0;
		}
		else if (m_cursor > getText().size())
		{
			m_cursor = getText().size();
		}
	}
}

} // ui
} // sharp
} // flat


