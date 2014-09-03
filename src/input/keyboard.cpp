#include "keyboard.h"

namespace flat
{
namespace input
{

Keyboard::Keyboard()
{
	SDL_GetKeyboardState(&m_numKeys);
	m_justPressedKeys = new bool[m_numKeys];
	m_justReleasedKeys = new bool[m_numKeys];
	clearEvents();
}

Keyboard::~Keyboard()
{
	delete m_justPressedKeys;
	delete m_justReleasedKeys;
}

bool Keyboard::isPressed(Key k) const
{
	const Uint8* pressedKeys = SDL_GetKeyboardState(NULL);
	
	if (k < m_numKeys)
		return (bool) pressedKeys[k];
		
	else
		return false;
}

bool Keyboard::isJustPressed(Key k) const
{
	if (k < m_numKeys)
		return m_justPressedKeys[k];
		
	else
		return false;
}

bool Keyboard::isJustReleased(Key k) const
{
	if (k < m_numKeys)
		return m_justReleasedKeys[k];
		
	else
		return false;
}

std::vector<Key> Keyboard::getPressedKeys() const
{
	std::vector<Key> pressedKeysVector;
	const Uint8* pressedKeys = SDL_GetKeyboardState(NULL);
	
	for (int k = 0; k < m_numKeys; k++)
		if (pressedKeys[k])
			pressedKeysVector.push_back((Key) k);
			
	return pressedKeysVector;
}

std::vector<Key> Keyboard::getJustPressedKeys() const
{
	std::vector<Key> justPressedKeysVector;
	
	for (int k = 0; k < m_numKeys; k++)
		if (m_justPressedKeys[k])
			justPressedKeysVector.push_back((Key) k);
			
	return justPressedKeysVector;
}

std::vector<Key> Keyboard::getJustReleasedKeys() const
{
	std::vector<Key> justReleasedKeysVector;
	
	for (int k = 0; k < m_numKeys; k++)
		if (m_justReleasedKeys[k])
			justReleasedKeysVector.push_back((Key) k);
			
	return justReleasedKeysVector;
}

void Keyboard::clearEvents()
{
	memset(m_justPressedKeys, 0, m_numKeys * sizeof(bool));
	memset(m_justReleasedKeys, 0, m_numKeys * sizeof(bool));
}

void Keyboard::addEvent(const SDL_Event& e)
{
	switch (e.type)
	{
		case SDL_KEYDOWN:
		if (!e.key.repeat && e.key.keysym.scancode < m_numKeys)
			m_justPressedKeys[e.key.keysym.scancode] = 1;
			
		break;

		case SDL_KEYUP:
		if (e.key.keysym.scancode < m_numKeys)
			m_justReleasedKeys[e.key.keysym.scancode] = 1;
			
		break;
	}
}

} // input
} // flat


