#include <SDL2/SDL.h>
#include "keyboardinputcontext.h"

namespace flat
{
namespace input
{
namespace context
{

KeyboardInputContext::KeyboardInputContext() :
	m_enableKeyRepeat(false)
{
	SDL_GetKeyboardState(&m_numKeys);
	m_pressedKeys.resize(m_numKeys);
	m_justPressedKeys.resize(m_numKeys);
	m_justReleasedKeys.resize(m_numKeys);

	clearAllEvents();
}

KeyboardInputContext& KeyboardInputContext::operator=(const KeyboardInputContext& other)
{
	// do not copy slots or callbacks could be called twice!
	// do not copy m_enableKeyRepeat either
	m_pressedKeys = other.m_pressedKeys;
	m_justPressedKeys = other.m_justPressedKeys;
	m_justReleasedKeys = other.m_justReleasedKeys;
	FLAT_ASSERT(m_numKeys == other.m_numKeys);
	return *this;
}

void KeyboardInputContext::addEvent(const SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_KEYDOWN:
		if ((!event.key.repeat || m_enableKeyRepeat) && m_justPressedKeys.isInRange(event.key.keysym.scancode))
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

	case SDL_TEXTINPUT:
		textEdited(event.text.text);
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

bool KeyboardInputContext::isPressed(Character c) const
{
	return isPressed(SDL_GetScancodeFromKey(c));
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

void KeyboardInputContext::setEnableTextInput(bool enable)
{
	if (enable)
	{
		SDL_StartTextInput();
	}
	else
	{
		SDL_StopTextInput();
	}
}

void KeyboardInputContext::copyStateFrom(const KeyboardInputContext & keyboardInputContext)
{
	m_pressedKeys = keyboardInputContext.m_pressedKeys;
	m_justPressedKeys = keyboardInputContext.m_justPressedKeys;
	m_justReleasedKeys = keyboardInputContext.m_justReleasedKeys;
	m_numKeys = keyboardInputContext.m_numKeys;
}

} // context
} // input
} // flat


