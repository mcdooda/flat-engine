#ifndef FLAT_INPUT_KEYBOARD_H
#define FLAT_INPUT_KEYBOARD_H

#include <vector>
#include <SDL2/SDL.h>

#include "../containers/array.h"

#define K(k) SDL_SCANCODE_##k

namespace flat
{
namespace input
{

typedef SDL_Scancode Key;

class Keyboard
{
	public:
		Keyboard();
		~Keyboard();
		
		bool isPressed(Key k) const;
		bool isJustPressed(Key k) const;
		bool isJustReleased(Key k) const;

		void getPressedKeys(std::vector<Key>& pressedKeys) const;
		void getJustPressedKeys(std::vector<Key>& justPressedKeys) const;
		void getJustReleasedKeys(std::vector<Key>& justReleasedKeys) const;

		void clearEvents();
		void addEvent(const SDL_Event& e);
		
	private:
		containers::Array<bool> m_justPressedKeys;
		containers::Array<bool> m_justReleasedKeys;
		int m_numKeys;
};

} // input
} // flat

#endif // FLAT_INPUT_KEYBOARD_H


