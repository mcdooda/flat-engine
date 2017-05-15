#ifndef FLAT_INPUT_CONTEXT_KEYBOARDINPUTCONTEXT_H
#define FLAT_INPUT_CONTEXT_KEYBOARDINPUTCONTEXT_H

#include <vector>

#include "../key.h"

#include "../../containers/array.h"

namespace flat
{
namespace input
{
namespace context
{

class KeyboardInputContext
{
	public:
		KeyboardInputContext();
		KeyboardInputContext(const KeyboardInputContext&) = delete;
		KeyboardInputContext(KeyboardInputContext&&) = delete;
		~KeyboardInputContext() = default;
		KeyboardInputContext& operator=(const KeyboardInputContext&) = default;
		
		void addEvent(const SDL_Event& event);
		void clearEvents();

		bool isPressed(Key key) const;
		bool isJustPressed(Key key) const;
		bool isJustReleased(Key key) const;

		void getPressedKeys(std::vector<Key>& pressedKeys) const;
		void getJustPressedKeys(std::vector<Key>& justPressedKeys) const;
		void getJustReleasedKeys(std::vector<Key>& justReleasedKeys) const;

	private:
		containers::Array<bool> m_pressedKeys;
		containers::Array<bool> m_justPressedKeys;
		containers::Array<bool> m_justReleasedKeys;
		int m_numKeys;
};

} // context
} // input
} // flat

#endif // FLAT_INPUT_CONTEXT_KEYBOARDINPUTCONTEXT_H


