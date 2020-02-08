#include <SDL2/SDL.h>

#include "input/context/gamepadinputcontext.h"

#include "misc/std.h"
#include "video/window.h"

namespace flat
{
namespace input
{
namespace context
{

GamepadInputContext::GamepadInputContext(SDL_JoystickID joystickId) :
	m_joystickId(joystickId)
{
	clearAllEvents();
}

bool GamepadInputContext::addEvent(const SDL_Event& event)
{
	if (event.cbutton.which != m_joystickId)
	{
		return false;
	}

	switch (event.type)
	{
	case SDL_CONTROLLERBUTTONDOWN:
		m_pressedButtons[event.cbutton.button] = true;
		m_justPressedButtons[event.cbutton.button] = true;
		return true;

	case SDL_CONTROLLERBUTTONUP:
		m_pressedButtons[event.cbutton.button] = false;
		m_justReleasedButtons[event.cbutton.button] = true;
		return true;

	case SDL_CONTROLLERAXISMOTION:
	{
		m_justMovedAxes[event.caxis.axis] = true;
		const float value = static_cast<float>(event.caxis.value) / 32768.f;
		m_axes[event.caxis.axis] = value;
		return true;
	}
	}

	return false;
}

void GamepadInputContext::clearFrameEvents()
{
	m_justPressedButtons.fill(false);
	m_justReleasedButtons.fill(false);
	m_justMovedAxes.fill(false);
}

void GamepadInputContext::clearAllEvents()
{
	m_pressedButtons.fill(false);
	m_axes.fill(0.f);
	clearFrameEvents();
}

bool GamepadInputContext::isPressed(GamepadButton button) const
{
	if (button < static_cast<int>(m_pressedButtons.size()))
	{
		return m_pressedButtons[button];
	}
	else
	{
		return false;
	}
}

bool GamepadInputContext::isJustPressed(GamepadButton button) const
{
	if (button < static_cast<int>(m_justPressedButtons.size()))
	{
		return m_justPressedButtons[button];
	}
	else
	{
		return false;
	}
}

bool GamepadInputContext::isJustReleased(GamepadButton button) const
{
	if (button < static_cast<int>(m_justReleasedButtons.size()))
	{
		return m_justReleasedButtons[button];
	}
	else
	{
		return false;
	}
}


bool GamepadInputContext::hasAxisJustMoved(GamepadAxis axis) const
{
	return m_justMovedAxes[axis];
}


float GamepadInputContext::getAxisValue(GamepadAxis axis) const
{
	return m_axes[axis];
}


flat::Vector2 GamepadInputContext::getLeftStickValue() const
{
	flat::Vector2 leftStickValue(m_axes[A(LEFTX)], m_axes[A(LEFTY)]);
	sanitizeStickValue(leftStickValue);
	return leftStickValue;
}


flat::Vector2 GamepadInputContext::getRightStickValue() const
{
	flat::Vector2 rightStickValue(m_axes[A(RIGHTX)], m_axes[A(RIGHTY)]);
	sanitizeStickValue(rightStickValue);
	return rightStickValue;
}

void GamepadInputContext::copyStateFrom(const GamepadInputContext& from)
{
	m_pressedButtons = from.m_pressedButtons;
	m_axes = from.m_axes;
}


void GamepadInputContext::sanitizeStickValue(flat::Vector2& stickValue)
{
	constexpr float MIN_STICK_VALUE = 0.2f;
	if (flat::length2(stickValue) < flat::square(MIN_STICK_VALUE))
	{
		stickValue.x = 0.f;
		stickValue.y = 0.f;
	}
}

} // context
} // input
} // flat


