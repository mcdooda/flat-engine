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

bool Keyboard::isPressed(Key k)
{
	const Uint8* pressedKeys = SDL_GetKeyboardState(NULL);
	
	int scanCode = SDL_GetScancodeFromKey(k);
	
	if (scanCode < m_numKeys)
		return (bool) pressedKeys[scanCode];
		
	else
		return false;
}

bool Keyboard::isJustPressed(Key k)
{
	return m_justPressedKeys[k];
}

bool Keyboard::isJustReleased(Key k)
{
	return m_justReleasedKeys[k];
}

std::vector<Key> Keyboard::getPressedKeys()
{
	std::vector<Key> pressedKeysVector;
	const Uint8* pressedKeys = SDL_GetKeyboardState(NULL);
	
	for (int k = 0; k < m_numKeys; k++)
		if (pressedKeys[k])
			pressedKeysVector.push_back((Key) k);
			
	return pressedKeysVector;
}

std::vector<Key> Keyboard::getJustPressedKeys()
{
	std::vector<Key> justPressedKeysVector;
	
	for (int k = 0; k < m_numKeys; k++)
		if (m_justPressedKeys[k])
			justPressedKeysVector.push_back((Key) k);
			
	return justPressedKeysVector;
}

std::vector<Key> Keyboard::getJustReleasedKeys()
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
		
		if (!e.key.repeat && e.key.keysym.sym < m_numKeys)
			m_justPressedKeys[e.key.keysym.sym] = 1;
			
		break;

		case SDL_KEYUP:
		
		if (e.key.keysym.sym < m_numKeys)
			m_justReleasedKeys[e.key.keysym.sym] = 1;
			
		break;
	}
}

} // input
} // flat


