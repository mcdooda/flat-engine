#ifndef FLAT_INPUT_CONTEXT_INPUTCONTEXT_H
#define FLAT_INPUT_CONTEXT_INPUTCONTEXT_H

#include "input/context/gamepadinputcontext.h"
#include "input/context/keyboardinputcontext.h"
#include "input/context/mouseinputcontext.h"
#include "input/context/windowinputcontext.h"
#include "input/gamepadbuttons.h"

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
		void clearFrameEvents();
		void clearAllEvents();

		void copyStateFrom(const InputContext& from);

		inline KeyboardInputContext& getKeyboardInputContext() { return m_keyboardInputContext; }
		inline const KeyboardInputContext& getKeyboardInputContext() const { return m_keyboardInputContext; }

		inline MouseInputContext& getMouseInputContext() { return m_mouseInputContext; }
		inline const MouseInputContext& getMouseInputContext() const { return m_mouseInputContext; }

		inline WindowInputContext& getWindowInputContext() { return m_windowInputContext; }
		inline const WindowInputContext& getWindowInputContext() const { return m_windowInputContext; }

		inline int getNumGamepadInputContexts() const { return static_cast<int>(m_gamepadInputContexts.size()); }
		inline GamepadInputContext* getGamepadInputContext(GamepadIndex index) { return index < m_gamepadInputContexts.size() ? &m_gamepadInputContexts[index] : nullptr; }
		inline const GamepadInputContext* getGamepadInputContext(GamepadIndex index) const { return index < m_gamepadInputContexts.size() ? &m_gamepadInputContexts[index] : nullptr; }

	private:
		void initGamepads();
		bool handleDeviceEvent(const SDL_Event& event);

	private:
		KeyboardInputContext m_keyboardInputContext;
		MouseInputContext m_mouseInputContext;
		WindowInputContext m_windowInputContext;
		std::vector<GamepadInputContext> m_gamepadInputContexts;
		bool m_isEnabled;
};

} // context
} // input
} // flat

#endif // FLAT_INPUT_CONTEXT_INPUTCONTEXT_H


