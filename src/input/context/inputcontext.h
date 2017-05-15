#ifndef FLAT_INPUT_CONTEXT_INPUTCONTEXT_H
#define FLAT_INPUT_CONTEXT_INPUTCONTEXT_H

#include "keyboardinputcontext.h"
#include "mouseinputcontext.h"
#include "windowinputcontext.h"

namespace flat
{
class Flat;
namespace input
{
namespace context
{

class InputContext
{
	public:
		InputContext() = delete;
		InputContext(const InputContext&) = delete;
		InputContext(InputContext&&) = delete;
		InputContext(Flat& flat);
		~InputContext() = default;
		InputContext& operator=(const InputContext&) = default;

		void addEvent(const SDL_Event& event);
		void clearEvents();

		inline KeyboardInputContext& getKeyboardInputContext() { return m_keyboardInputContext; }
		inline const KeyboardInputContext& getKeyboardInputContext() const { return m_keyboardInputContext; }

		inline MouseInputContext& getMouseInputContext() { return m_mouseInputContext; }
		inline const MouseInputContext& getMouseInputContext() const { return m_mouseInputContext; }

		inline WindowInputContext& getWindowInputContext() { return m_windowInputContext; }
		inline const WindowInputContext& getWindowInputContext() const { return m_windowInputContext; }

	private:
		KeyboardInputContext m_keyboardInputContext;
		MouseInputContext m_mouseInputContext;
		WindowInputContext m_windowInputContext;
};

} // context
} // input
} // flat

#endif // FLAT_INPUT_CONTEXT_INPUTCONTEXT_H


