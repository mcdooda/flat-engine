#ifndef FLAT_INPUT_CONTEXT_KEYBOARDINPUTCONTEXT_H
#define FLAT_INPUT_CONTEXT_KEYBOARDINPUTCONTEXT_H

#include <vector>

#include "../key.h"

#include "../../containers/array.h"
#include "../../misc/slot.h"

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
		KeyboardInputContext& operator=(const KeyboardInputContext& other);
		
		void addEvent(const SDL_Event& event);
		void clearFrameEvents();
		void clearAllEvents();

		bool isPressed(Key key) const;
		bool isJustPressed(Key key) const;
		bool isJustReleased(Key key) const;

		void getPressedKeys(std::vector<Key>& pressedKeys) const;
		void getJustPressedKeys(std::vector<Key>& justPressedKeys) const;
		void getJustReleasedKeys(std::vector<Key>& justReleasedKeys) const;

		inline void setEnableKeyRepeat(bool enableKeyRepeat) { m_enableKeyRepeat = enableKeyRepeat; }
		void setEnableTextInput(bool enable);

		void copyStateFrom(const KeyboardInputContext& keyboardInputContext);

	public:
		Slot<Key> keyJustPressed;
		Slot<Key> keyJustReleased;
		Slot<const std::string&> textEdited;

	private:
		containers::Array<bool> m_pressedKeys;
		containers::Array<bool> m_justPressedKeys;
		containers::Array<bool> m_justReleasedKeys;
		int m_numKeys;

		bool m_enableKeyRepeat;
};

} // context
} // input
} // flat

#endif // FLAT_INPUT_CONTEXT_KEYBOARDINPUTCONTEXT_H


