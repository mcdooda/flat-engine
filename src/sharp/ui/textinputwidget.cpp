#include <cctype>
#include <limits>
#include "sharp/ui/textinputwidget.h"

#include "flat.h"

namespace flat
{
namespace sharp
{
namespace ui
{

TextInputWidget::TextInputWidget(Flat& flat, const std::shared_ptr<const video::font::Font>& font) : TextWidget(font),
	m_flat(flat),
	m_cursorIndex(0),
	m_selectionIndex(0)
{
	setFocusable(true);
	enterFocus.on(this, &TextInputWidget::enteredFocus);
	leaveFocus.on(this, &TextInputWidget::leftFocus);
	mouseDown.on(this, &TextInputWidget::onMouseDown);
	mouseMove.on(this, &TextInputWidget::onMouseMove);
	paste.on(this, &TextInputWidget::onPaste);
	copy.on(this, &TextInputWidget::onCopy);
}

TextInputWidget::~TextInputWidget()
{
	enterFocus.off(this);
	leaveFocus.off(this);
	mouseDown.off(this);
}

void TextInputWidget::draw(const render::RenderSettings& renderSettings, const ScissorRectangle& parentScissor) const
{
	drawSelection(renderSettings, m_cursorIndex, m_selectionIndex);
	TextWidget::draw(renderSettings, parentScissor);
	drawCursor(renderSettings, m_cursorIndex);
}

CursorType TextInputWidget::getCursorType() const
{
	return CURSOR(IBEAM);
}

void TextInputWidget::replaceText(const std::string& text)
{
	setText(text);
	m_cursorIndex = std::min(m_cursorIndex, text.size());
	unselect();
}

bool TextInputWidget::enteredFocus(Widget* widget)
{
	selectAll();
	m_inputContext = std::make_shared<input::context::InputContext>(m_flat);
	input::context::KeyboardInputContext& keyboardInputContext = m_inputContext->getKeyboardInputContext();
	keyboardInputContext.setEnableKeyRepeat(true);
	keyboardInputContext.keyJustPressed.on(this, &TextInputWidget::keyJustPressed);
	keyboardInputContext.textEdited.on(this, &TextInputWidget::textEdited);
	keyboardInputContext.setEnableTextInput(true);
	m_flat.input->pushContext(m_inputContext);
	return true;
}

bool TextInputWidget::leftFocus(Widget* widget)
{
	if(hasSelectedText())
	{
		unselect();
	}
	input::context::KeyboardInputContext& keyboardInputContext = m_inputContext->getKeyboardInputContext();
	keyboardInputContext.keyJustPressed.off(this);
	keyboardInputContext.textEdited.off(this);
	keyboardInputContext.setEnableTextInput(false);
	m_flat.input->popContext(m_inputContext);
	m_inputContext.reset();
	return true;
}

bool TextInputWidget::onMouseDown(Widget* widget, bool& eventHandled)
{
	eventHandled = true;
	if (hasSelectedText())
	{
		unselect();
	}
	auto& mouse = m_flat.input->mouse;
	const flat::Vector2 pos = getRelativePosition(mouse->getPosition());
	moveCursorAt(getCursorIndexFromPosition(pos));
	if (mouse->isJustDoubleClicked(M(LEFT)))
	{
		moveCursorAt(nextWordFrom(m_cursorIndex));
		selectTo(previousWordFrom(m_cursorIndex));
	}
	return true;
}

bool TextInputWidget::onMouseMove(Widget* widget, bool& eventHandled)
{
	auto& mouse = m_flat.input->mouse;
	if (hasFocus() && mouse->isPressed(M(LEFT)))
	{
		eventHandled = true;
		const flat::Vector2 pos = getRelativePosition(mouse->getPosition());
		m_cursorIndex = getCursorIndexFromPosition(pos);
		selectTo(m_selectionIndex);
	}
	return true;
}

bool TextInputWidget::onPaste(Widget* widget, const std::string& text)
{
	textEdited(text);
	return true;
}

bool TextInputWidget::onCopy(Widget* widget, std::string& copied)
{
	copied = getSelectedText();
	return true;
}

bool TextInputWidget::keyJustPressed(input::Key key)
{
	input::Character character = input::Keyboard::getCharacter(key);
	std::string text = getText();
	const CursorIndex currentIndex = m_selectionIndex;
	const bool ctrlPressed = m_inputContext->getKeyboardInputContext().isPressed(K(LCTRL)) || m_inputContext->getKeyboardInputContext().isPressed(K(RCTRL));
	const bool shiftPressed = m_inputContext->getKeyboardInputContext().isPressed(K(LSHIFT)) || m_inputContext->getKeyboardInputContext().isPressed(K(RSHIFT));
	if (character == C(BACKSPACE) && !text.empty())
	{
		if (hasSelectedText())
		{
			changeSelectedText("");
			valueChanged(this, getText());
		}
		else if(m_cursorIndex > 0)
		{
			FLAT_ASSERT(m_cursorIndex - 1 < text.size());
			text = text.erase(m_cursorIndex - 1, 1);
			if (text != getText())
			{
				moveCursor(-1);
				setText(text);
				valueChanged(this, text);
			}
		}
	}
	else if (character == C(DELETE) && !text.empty())
	{
		if (hasSelectedText())
		{
			changeSelectedText("");
			unselect();
			valueChanged(this, getText());
		}
		else if (m_cursorIndex < text.size())
		{
			text = text.erase(m_cursorIndex, 1);
			if (text != getText())
			{
				setText(text);
				valueChanged(this, text);
			}
		}
	}
	else if (character == C(RETURN) || character == C(KP_ENTER))
	{
		if (shiftPressed)
		{
			changeSelectedText("\n");
		}
		else
		{
			unselect();
			submit(this);
		}
	}
	else
	{
		// cursor management
		if (character == 'a' && ctrlPressed)
		{
			selectAll();
		}
		else if (character == C(HOME))
		{
			moveCursorAt(0);
		}
		else if (character == C(END))
		{
			moveCursorAt(text.size());
		}
		else if (character == C(LEFT))
		{
			if (ctrlPressed)
			{
				moveCursorAt(previousWordFrom(m_cursorIndex));
			}
			else
			{
				if (hasSelectedText() && !shiftPressed)
				{
					m_cursorIndex = std::min(m_cursorIndex, m_selectionIndex);
					unselect();
				}
				else
				{
					moveCursor(-1);
				}
			}
		}
		else if (character == C(RIGHT))
		{
			if (ctrlPressed)
			{
				moveCursorAt(nextWordFrom(m_cursorIndex));
			}
			else
			{
				if (hasSelectedText() && !shiftPressed)
				{
					m_cursorIndex = std::max(m_cursorIndex, m_selectionIndex);
					unselect();
				}
				else
				{
					moveCursor(1);
				}
			}
		}
		else if (character == C(DOWN))
		{
			if (!shiftPressed)
			{
				unselect();
			}
			moveCursorAt(nextLineFrom(m_cursorIndex));
		}
		else if (character == C(UP))
		{
			if (!shiftPressed)
			{
				unselect();
			}
			moveCursorAt(previousLineFrom(m_cursorIndex));
		}
		if (shiftPressed)
		{
			selectTo(currentIndex);
		}
	}
	return true;
}

bool TextInputWidget::textEdited(const std::string& text)
{
	if (!text.empty())
	{
		changeSelectedText(text);
		valueChanged(this, getText());
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
		m_selectionIndex = m_cursorIndex;
	}
}

void TextInputWidget::moveCursorAt(CursorIndex index)
{
	FLAT_ASSERT(index >= 0 && index <= getText().size());
	m_cursorIndex = index;
	unselect();
}

void TextInputWidget::selectTo(CursorIndex to)
{
	FLAT_ASSERT(to >= 0 && to <= getText().size())
	m_selectionIndex = to;
	setColor(0u, static_cast<unsigned int>(getText().size()), getTextColor());
	const unsigned int first = static_cast<unsigned int>(std::min(m_selectionIndex, m_cursorIndex));
	const unsigned int last = static_cast<unsigned int>(std::max(m_selectionIndex, m_cursorIndex));
	setColor(first, last, video::Color::WHITE);
}

void TextInputWidget::unselect()
{
	setColor(0u, static_cast<unsigned int>(getText().size()), getTextColor());
	m_selectionIndex = m_cursorIndex;
}

void TextInputWidget::selectAll()
{
	moveCursorAt(getText().size());
	selectTo(0);
}

std::string TextInputWidget::replaceSelectedText(const std::string& text)
{
	CursorIndex min = std::min(m_cursorIndex, m_selectionIndex);
	CursorIndex max = std::max(m_cursorIndex, m_selectionIndex);
	std::string currentText = getText();
	currentText.erase(min, max - min);
	currentText.insert(min, text);
	return currentText;
}

void TextInputWidget::changeSelectedText(const std::string& text)
{
	std::string newText = replaceSelectedText(text);
	if (newText != getText())
	{
		setText(newText);
	}
	moveCursorAt(std::min(m_cursorIndex, m_selectionIndex) + text.size());
}

std::string TextInputWidget::getSelectedText() const
{
	CursorIndex min = std::min(m_cursorIndex, m_selectionIndex);
	CursorIndex max = std::max(m_cursorIndex, m_selectionIndex);
	return getText().substr(min, max - min);
}

TextInputWidget::CursorIndex TextInputWidget::nextWordFrom(CursorIndex index) const
{
	const std::string& text = getText();
	CursorIndex nextWordIndex = text.find_first_of(" \n", index);
	if (nextWordIndex == index)
	{
		return index + 1;
	}
	else if (nextWordIndex != std::string::npos)
	{
		return nextWordIndex;
	}
	else
	{
		return text.size();
	}
}

TextInputWidget::CursorIndex TextInputWidget::previousWordFrom(CursorIndex index) const
{
	if (index > 0)
	{
		CursorIndex previousSpaceIndex = getText().find_last_of(" \n", index - 1) + 1;
		if (previousSpaceIndex > 0 && previousSpaceIndex == index)
		{
			return index - 1;
		}
		else if (previousSpaceIndex != std::string::npos)
		{
			return previousSpaceIndex;
		}
	}
	return 0;
}

TextInputWidget::CursorIndex TextInputWidget::nextLineFrom(CursorIndex index) const
{
	Vector2 pos = getCursorPositionFromIndex(index);
	pos.y -= getLineHeight();
	return getCursorIndexFromPosition(pos);
}

TextInputWidget::CursorIndex TextInputWidget::previousLineFrom(CursorIndex index) const
{
	Vector2 pos = getCursorPositionFromIndex(index);
	pos.y += getLineHeight();
	return getCursorIndexFromPosition(pos);
}

bool TextInputWidget::hasSelectedText()
{
	return m_cursorIndex != m_selectionIndex;
}

flat::Vector2i TextInputWidget::getCursorPositionFromIndex(CursorIndex cursorIndex) const
{
	const std::string& text = getText();

	const size_t textLength = text.size();
	FLAT_ASSERT_MSG(0 <= cursorIndex && cursorIndex <= textLength, "the cursor index is out of the string's range");
	if ((cursorIndex != 0 && cursorIndex == textLength) || text[cursorIndex] == '\n')
		return getCursorEndingFromIndex(cursorIndex - 1);

	if (text.size() == 0)
		return { 0, String::getComputedSize().y};
	const std::vector<CharMetrics>& infos = getCharMetrics();
	return infos[cursorIndex].position;
}

flat::Vector2i TextInputWidget::getCursorEndingFromIndex(CursorIndex cursorIndex) const
{
	const std::string& text = getText();
	const size_t textLength = text.size();
	FLAT_ASSERT_MSG(0 <= cursorIndex && cursorIndex <= textLength, "the cursor index is out of the string's range");
	const std::vector<CharMetrics>& infos = getCharMetrics();
	flat::Vector2 result = infos[cursorIndex].position;
	result.x += infos[cursorIndex].size.x;
	return result;
}

TextInputWidget::CursorIndex TextInputWidget::getCursorIndexFromPosition(flat::Vector2 pos) const
{
	const std::string& text = getText();
	const size_t textLength = text.size();
	CursorIndex result = 0;
	if (textLength == 0)
	{
		return result;
	}

	int y = static_cast<int>((String::getComputedSize().y - pos.y) / getLineHeight());
	if (y < 0)
		y = 0;
	size_t startX = 0;
	size_t endX = 0;
	while (y > 0)
	{
		size_t next = text.find('\n', startX);
		if (next != std::string::npos)
			startX = next + 1;
		y--;
	}

	endX = text.find('\n', startX + 1);
	if (endX == std::string::npos)
		endX = textLength;
	if (pos.x < getCursorPositionFromIndex(startX).x)
	{
		result = startX;
	}
	else if (pos.x > getCursorPositionFromIndex(endX).x)
	{
		result = endX;
	}
	else
	{
		size_t minXIndex = startX;
		size_t maxXIndex = endX;
		while (minXIndex + 1 < maxXIndex)
		{
			size_t centerIndex = (minXIndex + maxXIndex) / 2;
			if (pos.x < getCursorPositionFromIndex(centerIndex).x)
			{
				maxXIndex = centerIndex;
			}
			else
			{
				minXIndex = centerIndex;
			}
		}

		if (pos.x - getCursorPositionFromIndex(minXIndex).x < getCursorPositionFromIndex(maxXIndex).x - pos.x)
		{
			return minXIndex;
		}
		else
		{
			return maxXIndex;
		}
	}
	return result;
}

void TextInputWidget::drawCursor(const render::RenderSettings& renderSettings, CursorIndex cursorIndex) const
{
	if (!hasFocus())
		return;

	renderSettings.modelMatrixUniform.set(m_transform);

	renderSettings.colorUniform.set(getTextColor());
	renderSettings.secondaryColorUniform.set(video::Color::BLACK);
	renderSettings.textureGivenUniform.set(false);
	renderSettings.vertexColorGivenUniform.set(false);

	const video::font::Font* font = getFont().get();
	FLAT_ASSERT(font != nullptr);

	const float characterHeight = font->getAtlasSize().y;
	const flat::Vector2 cursorPos = getCursorPositionFromIndex(cursorIndex);
	std::array<String::CharacterVertex, 2> cursorVertices = {
		String::CharacterVertex(cursorPos.x + 1, cursorPos.y),
		String::CharacterVertex(cursorPos.x + 1, cursorPos.y - characterHeight)
	};

	glLineWidth(1);

	glVertexAttribPointer(renderSettings.positionAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(CharacterVertex), reinterpret_cast<const float*>(&cursorVertices[0]));
	glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(cursorVertices.size()));
}

void TextInputWidget::drawSelection(const render::RenderSettings& renderSettings, CursorIndex first, CursorIndex last) const
{
	if (!hasFocus() || first == last)
		return;

	if (first > last)
	{
		CursorIndex tmp = first;
		first = last;
		last = tmp;
	}

	renderSettings.modelMatrixUniform.set(m_transform);

	renderSettings.colorUniform.set(video::Color(uint32_t(0x4286f4FF)));
	renderSettings.secondaryColorUniform.set(video::Color::BLACK);
	renderSettings.textureGivenUniform.set(false);
	renderSettings.vertexColorGivenUniform.set(false);

	const video::font::Font* font = getFont().get();
	FLAT_ASSERT(font != nullptr);

	const std::string& text = getText();
	const float characterHeight = getLineHeight();
	Vector2 firstPos = getCursorPositionFromIndex(first);
	Vector2 lastPos = getCursorPositionFromIndex(last);

	glLineWidth(1);
	size_t nextLineIndex = text.find('\n', first);
	while (nextLineIndex < last)
	{
		Vector2 lineEnding = getCursorEndingFromIndex(nextLineIndex - 1);
		std::array<String::CharacterVertex, 6> cursorVertices = {
			String::CharacterVertex(firstPos.x, firstPos.y),
			String::CharacterVertex(lineEnding.x + 3, firstPos.y),
			String::CharacterVertex(firstPos.x, firstPos.y - characterHeight),
			String::CharacterVertex(firstPos.x, firstPos.y - characterHeight),
			String::CharacterVertex(lineEnding.x + 3, firstPos.y),
			String::CharacterVertex(lineEnding.x + 3, firstPos.y - characterHeight)
		};
		first = nextLineIndex + 1;
		firstPos = getCursorPositionFromIndex(first);

		glVertexAttribPointer(renderSettings.positionAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(CharacterVertex), reinterpret_cast<const float*>(&cursorVertices[0]));
		glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(cursorVertices.size()));
		nextLineIndex = text.find('\n', first + 1);
	}

	std::array<String::CharacterVertex, 6> cursorVertices = {
		String::CharacterVertex(firstPos.x, firstPos.y),
		String::CharacterVertex(lastPos.x, lastPos.y),
		String::CharacterVertex(firstPos.x, firstPos.y - characterHeight),
		String::CharacterVertex(firstPos.x, firstPos.y - characterHeight),
		String::CharacterVertex(lastPos.x, lastPos.y),
		String::CharacterVertex(lastPos.x, firstPos.y - characterHeight)
	};

	glVertexAttribPointer(renderSettings.positionAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(CharacterVertex), reinterpret_cast<const float*>(&cursorVertices[0]));
	glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(cursorVertices.size()));
}

} // ui
} // sharp
} // flat


