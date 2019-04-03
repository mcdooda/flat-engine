#ifndef FLAT_INPUT_KEYBOARD_H
#define FLAT_INPUT_KEYBOARD_H

#include <vector>
#include <memory>

#include "input/key.h"

namespace flat
{
namespace input
{
namespace context
{
class InputContext;
}

class Keyboard
{
	public:
		Keyboard() = delete;
		Keyboard(const Keyboard&) = delete;
		Keyboard(Keyboard&&) = delete;
		Keyboard(const std::shared_ptr<context::InputContext>& globalInputContext);
		~Keyboard() = default;
		
		bool isPressed(Key key) const;
		bool isPressed(Character character) const;
		bool isJustPressed(Key key) const;
		bool isJustPressed(Character character) const;
		bool isJustReleased(Key key) const;

		void getPressedKeys(std::vector<Key>& pressedKeys) const;
		void getJustPressedKeys(std::vector<Key>& justPressedKeys) const;
		void getJustReleasedKeys(std::vector<Key>& justReleasedKeys) const;

		static Key getKey(input::Character character);
		static Character getCharacter(input::Key key);

	private:
		std::shared_ptr<context::InputContext> m_globalInputContext;
};

} // input
} // flat

#endif // FLAT_INPUT_KEYBOARD_H


