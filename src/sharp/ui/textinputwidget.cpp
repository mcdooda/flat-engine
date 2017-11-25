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

void TextInputWidget::draw(const render::RenderSettings& renderSettings, const ScissorRectangle& parentScissor) const
{
	TextWidget::draw(renderSettings, parentScissor);
	drawCursor(renderSettings, static_cast<CursorIndex>(getText().size()));
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
	else if (key == K(RETURN) || key == K(KP_ENTER))
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


