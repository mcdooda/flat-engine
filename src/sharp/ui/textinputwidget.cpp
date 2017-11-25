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
	m_cursorIndex(0)
{
	enterFocus.on(this, &TextInputWidget::enteredFocus);
	leaveFocus.on(this, &TextInputWidget::leftFocus);
	mouseDown.on(this, &TextInputWidget::onMouseDown);
}

TextInputWidget::~TextInputWidget()
{
	enterFocus.off(this);
	leaveFocus.off(this);
	mouseDown.off(this);
}

void TextInputWidget::draw(const render::RenderSettings& renderSettings, const ScissorRectangle& parentScissor) const
{
	TextWidget::draw(renderSettings, parentScissor);
	drawCursor(renderSettings, m_cursorIndex);
}

FLAT_OPTIMIZE_OFF()
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
FLAT_OPTIMIZE_ON()

bool TextInputWidget::leftFocus(Widget* widget)
{
	m_inputContext->getKeyboardInputContext().keyJustPressed.off(this);
	m_inputContext->getKeyboardInputContext().textEdited.off(this);
	m_inputContext->getKeyboardInputContext().setEnableTextInput(false);
	m_flat.input->popContext(m_inputContext);
	m_inputContext.reset();
	return true;
}

bool TextInputWidget::onMouseDown(Widget* widget, bool&)
{
	auto& mouse = m_flat.input->mouse;
	const float mouseX = getRelativePosition(mouse->getPosition()).x;
	m_cursorIndex = getCursorIndexFromPosition(mouseX);
	return true;
}

bool TextInputWidget::keyJustPressed(input::Key key)
{
	std::string text = getText();
	if (key == K(BACKSPACE) && !text.empty())
	{
		if (m_cursorIndex > 0)
		{
			FLAT_ASSERT(m_cursorIndex - 1 < text.size());
			text = text.erase(m_cursorIndex - 1, 1);
			if (text != getText())
			{
				moveCursor(-1);
				setText(text);
				valueChanged(this);
			}
		}
	}
	else if (key == K(DELETE) && !text.empty())
	{
		if (m_cursorIndex < text.size())
		{
			text = text.erase(m_cursorIndex, 1);
			if (text != getText())
			{
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
	currentText.insert(m_cursorIndex, text);
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
	if (offset > 0 || m_cursorIndex >= static_cast<CursorIndex>(std::abs(offset)))
	{
		m_cursorIndex += offset;
		if (m_cursorIndex < 0)
		{
			m_cursorIndex = 0;
		}
		else if (m_cursorIndex > getText().size())
		{
			m_cursorIndex = getText().size();
		}
	}
}

float TextInputWidget::getCursorPositionFromIndex(CursorIndex cursorIndex) const
{
	const size_t textLength = getText().size();
	FLAT_ASSERT_MSG(0 <= cursorIndex && cursorIndex <= textLength, "the cursor index is out of the string's range");
	const std::vector<CharacterVertex>& vertices = getVertices();
	if (cursorIndex == 0)
	{
		return 0.f;
	}
	else
	{
		return vertices[(cursorIndex - 1) * 6 + 1].x;
	}
}

TextInputWidget::CursorIndex TextInputWidget::getCursorIndexFromPosition(float x) const
{
	const size_t textLength = getText().size();
	if (textLength == 0)
	{
		return 0;
	}

	const std::vector<CharacterVertex>& vertices = getVertices();
	if (x < vertices[0].x)
	{
		return 0;
	}
	else if (x > vertices[vertices.size() - 1].x)
	{
		return textLength;
	}
	else
	{
		CursorIndex minIndex = 0;
		CursorIndex maxIndex = textLength;
		while (minIndex + 1 < maxIndex)
		{
			CursorIndex centerIndex = (minIndex + maxIndex) / 2;
			float centerCharacterPositionX = getCursorPositionFromIndex(centerIndex);
			if (x < centerCharacterPositionX)
			{
				maxIndex = centerIndex;
			}
			else
			{
				minIndex = centerIndex;
			}
		}

		if (x - getCursorPositionFromIndex(minIndex) < getCursorPositionFromIndex(maxIndex) - x)
		{
			return minIndex;
		}
		else
		{
			return maxIndex;
		}
	}
}

void TextInputWidget::drawCursor(const render::RenderSettings& renderSettings, CursorIndex cursorIndex) const
{
	if (!hasFocus())
		return;

	renderSettings.modelMatrixUniform.set(m_transform);

	renderSettings.colorUniform.set(getTextColor());
	renderSettings.secondaryColorUniform.set(video::Color::BLACK);
	renderSettings.textureGivenUniform.set(false);

	const video::font::Font* font = getFont().get();
	FLAT_ASSERT(font != nullptr);

	const float characterHeight = font->getAtlasSize().y;
	const float cursorX = getCursorPositionFromIndex(cursorIndex);

	std::array<String::CharacterVertex, 2> cursorVertices = {
		String::CharacterVertex(cursorX, 0.f),
		String::CharacterVertex(cursorX, characterHeight)
	};

	glLineWidth(1);

	glEnableVertexAttribArray(renderSettings.positionAttribute);
	glVertexAttribPointer(renderSettings.positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const float*>(&cursorVertices[0]));

	glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(cursorVertices.size()));

	glDisableVertexAttribArray(renderSettings.positionAttribute);
}

} // ui
} // sharp
} // flat


