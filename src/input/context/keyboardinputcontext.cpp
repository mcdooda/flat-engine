#include <SDL2/SDL.h>
#include "keyboardinputcontext.h"

namespace flat
{
namespace input
{
namespace context
{

KeyboardInputContext::KeyboardInputContext()
{
	SDL_GetKeyboardState(&m_numKeys);
	m_pressedKeys.resize(m_numKeys);
	m_justPressedKeys.resize(m_numKeys);
	m_justReleasedKeys.resize(m_numKeys);

	clearAllEvents();
}

void KeyboardInputContext::addEvent(const SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_KEYDOWN:
		if (!event.key.repeat && m_justPressedKeys.isInRange(event.key.keysym.scancode))
		{
			m_pressedKeys[event.key.keysym.scancode] = true;
			m_justPressedKeys[event.key.keysym.scancode] = true;
			keyJustPressed(event.key.keysym.scancode);
		}
		break;

	case SDL_KEYUP:
		if (m_justReleasedKeys.isInRange(event.key.keysym.scancode))
		{
			m_pressedKeys[event.key.keysym.scancode] = false;
			m_justReleasedKeys[event.key.keysym.scancode] = true;
			keyJustReleased(event.key.keysym.scancode);
		}
		break;
	}
}

void KeyboardInputContext::clearFrameEvents()
{
	m_justPressedKeys.memset(0);
	m_justReleasedKeys.memset(0);
}

void KeyboardInputContext::clearAllEvents()
{
	m_pressedKeys.memset(0);
	clearFrameEvents();
}

bool KeyboardInputContext::isPressed(Key key) const
{
	if (key < m_numKeys)
	{
		return m_pressedKeys[key];
	}
	else
	{
		return false;
	}
}

bool KeyboardInputContext::isJustPressed(Key key) const
{
	if (m_justPressedKeys.isInRange(key))
	{
		return m_justPressedKeys[key];
	}
	else
	{
		return false;
	}
}

bool KeyboardInputContext::isJustReleased(Key key) const
{
	if (m_justPressedKeys.isInRange(key))
	{
		return m_justReleasedKeys[key];
	}
	else
	{
		return false;
	}
}

void KeyboardInputContext::getPressedKeys(std::vector<Key>& pressedKeys) const
{
	pressedKeys.clear();

	for (int key = 0; key < m_numKeys; ++key)
	{
		if (m_pressedKeys[key])
		{
			pressedKeys.push_back(static_cast<Key>(key));
		}
	}
}

void KeyboardInputContext::getJustPressedKeys(std::vector<Key>& justPressedKeys) const
{
	justPressedKeys.clear();

	for (int key = 0; key < m_numKeys; ++key)
	{
		if (m_justPressedKeys[key])
		{
			justPressedKeys.push_back(static_cast<Key>(key));
		}
	}
}

void KeyboardInputContext::getJustReleasedKeys(std::vector<Key>& justReleasedKeys) const
{
	justReleasedKeys.clear();

	for (int key = 0; key < m_numKeys; ++key)
	{
		if (m_justReleasedKeys[key])
		{
			justReleasedKeys.push_back(static_cast<Key>(key));
		}
	}
}

} // context
} // input
} // flat


