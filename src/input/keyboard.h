#ifndef FLAT_INPUT_KEYBOARD_H
#define FLAT_INPUT_KEYBOARD_H

#include <vector>
#include <SDL2/SDL.h>

#define K(k) SDLK_##k

namespace flat
{
namespace input
{

typedef SDL_Keycode Key;

class Keyboard
{
	public:
		Keyboard();
		~Keyboard();
		
		bool isPressed(Key k);
		bool isJustPressed(Key k);
		bool isJustReleased(Key k);

		std::vector<Key> getPressedKeys();
		std::vector<Key> getJustPressedKeys();
		std::vector<Key> getJustReleasedKeys();

		void clearEvents();
		void addEvent(const SDL_Event& e);
		
	private:
		int m_numKeys;
		bool* m_justPressedKeys;
		bool* m_justReleasedKeys;
};

} // input
} // flat

#endif // FLAT_INPUT_KEYBOARD_H


