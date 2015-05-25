#include "keyboard.h"

namespace flat
{
namespace input
{

Keyboard::Keyboard()
{
	SDL_GetKeyboardState(&m_numKeys);
	m_justPressedKeys.resize(m_numKeys);
	m_justReleasedKeys.resize(m_numKeys);
	clearEvents();
}

Keyboard::~Keyboard()
{
	
}

bool Keyboard::isPressed(Key k) const
{
	const Uint8* pressedKeys = SDL_GetKeyboardState(nullptr);
	
	if (k < m_numKeys)
		return static_cast<bool>(pressedKeys[k]);
		
	else
		return false;
}

bool Keyboard::isJustPressed(Key k) const
{
	if (m_justPressedKeys.isInRange(k))
		return m_justPressedKeys[k];
		
	else
		return false;
}

bool Keyboard::isJustReleased(Key k) const
{
	if (m_justPressedKeys.isInRange(k))
		return m_justReleasedKeys[k];
		
	else
		return false;
}

void Keyboard::getPressedKeys(std::vector<Key>& pressedKeys) const
{
	const Uint8* pressedKeysMap = SDL_GetKeyboardState(nullptr);
	
	pressedKeys.clear();
	
	for (int k = 0; k < m_numKeys; k++)
		if (pressedKeysMap[k])
			pressedKeys.push_back((Key) k);
}

void Keyboard::getJustPressedKeys(std::vector<Key>& justPressedKeys) const
{
	justPressedKeys.clear();
	
	for (int k = 0; k < m_numKeys; k++)
		if (m_justPressedKeys[k])
			justPressedKeys.push_back((Key) k);
}

void Keyboard::getJustReleasedKeys(std::vector<Key>& justReleasedKeys) const
{
	justReleasedKeys.clear();
	
	for (int k = 0; k < m_numKeys; k++)
		if (m_justReleasedKeys[k])
			justReleasedKeys.push_back((Key) k);
}

void Keyboard::clearEvents()
{
	m_justPressedKeys.memset(0);
	m_justReleasedKeys.memset(0);
}

void Keyboard::addEvent(const SDL_Event& e)
{
	switch (e.type)
	{
		case SDL_KEYDOWN:
		if (!e.key.repeat && e.key.keysym.scancode < m_numKeys)
			m_justPressedKeys[e.key.keysym.scancode] = true;
			
		break;

		case SDL_KEYUP:
		if (e.key.keysym.scancode < m_numKeys)
			m_justReleasedKeys[e.key.keysym.scancode] = true;
			
		break;
	}
}

} // input
} // flat


