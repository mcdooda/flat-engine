#ifndef FLAT_INPUT_KEYBOARD_H
#define FLAT_INPUT_KEYBOARD_H

#include <vector>
#include <SDL2/SDL.h>

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

		std::vector<Key> getPressedKeys() const;
		std::vector<Key> getJustPressedKeys() const;
		std::vector<Key> getJustReleasedKeys() const;

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


