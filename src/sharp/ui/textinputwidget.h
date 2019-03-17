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
		using CursorIndex = size_t;

	public:
		TextInputWidget() = delete;
		TextInputWidget(const TextInputWidget&) = delete;
		TextInputWidget(TextInputWidget&&) = delete;
		TextInputWidget(Flat& flat, const std::shared_ptr<const video::font::Font>& font);
		virtual ~TextInputWidget();
		TextInputWidget& operator=(const TextInputWidget&) = delete;

		void draw(const render::RenderSettings& renderSettings, const ScissorRectangle& parentScissor) const override;

		CursorType getCursorType() const override;

	public:
		Slot<Widget*> valueChanged;
		Slot<Widget*> submit;

	protected:
		virtual bool textEdited(const std::string& text);
		virtual bool keyJustPressed(input::Key key);
		void replaceText(const std::string& text);
		std::string replaceSelectedText(const std::string& text);
		virtual bool leftFocus(Widget* widget);

	private:
		bool enteredFocus(Widget* widget);
		bool onMouseDown(Widget* widget, bool& eventHandled);
		bool onMouseMove(Widget* widget, bool& eventHandled);
		bool onPaste(Widget* widget, const std::string& text);
		bool onCopy(Widget* widget, std::string& copied);
		void moveCursor(int offset);
		void moveCursorAt(CursorIndex index);
		void selectTo(CursorIndex to);
		void unselect();
		void selectAll();
		bool hasSelectedText();
		void changeSelectedText(const std::string& text);
		std::string getSelectedText() const;
		CursorIndex nextWordFrom(CursorIndex index) const;
		CursorIndex previousWordFrom(CursorIndex index) const;

		flat::Vector2 getCursorPositionFromIndex(CursorIndex cursorIndex) const;
		flat::Vector2 getCursorEndingFromIndex(CursorIndex cursorIndex) const;
		CursorIndex getCursorIndexFromPosition(flat::Vector2 pos) const;
		void drawCursor(const render::RenderSettings& renderSettings, CursorIndex cursorIndex) const;
		void drawSelection(const render::RenderSettings& renderSettings, CursorIndex first, CursorIndex last) const;

	private:
		Flat& m_flat;
		std::shared_ptr<input::context::InputContext> m_inputContext;
		CursorIndex m_cursorIndex;
		CursorIndex m_selectionIndex;
};

} // ui
} // sharp
} // flat

#endif // FLAT_SHARP_UI_TEXTINPUTWIDGET_H


