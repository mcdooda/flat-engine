#include <SDL2/SDL.h>

#include "mouseinputcontext.h"

#include "../../video/window.h"

namespace flat
{
namespace input
{
namespace context
{

MouseInputContext::MouseInputContext(video::Window* videoWindow) :
	m_videoWindow(videoWindow)
{
	m_pressedButtons.fill(false);
	clearAllEvents();
}

void MouseInputContext::addEvent(const SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		m_position.x = static_cast<float>(event.button.x);
		m_position.y = m_videoWindow->getSize().y - event.button.y;
		m_pressedButtons[event.button.button] = true;
		m_justPressedButtons[event.button.button] = true;
		if (event.button.clicks > 1)
		{
			m_justDoubleClickedButtons[event.button.button] = true;
		}
		break;

	case SDL_MOUSEBUTTONUP:
		m_position.x = static_cast<float>(event.button.x);
		m_position.y = m_videoWindow->getSize().y - event.button.y;
		m_pressedButtons[event.button.button] = false;
		m_justReleasedButtons[event.button.button] = true;
		break;

	case SDL_MOUSEMOTION:
		m_position.x = static_cast<float>(event.button.x);
		m_position.y = m_videoWindow->getSize().y - event.button.y;
		m_moved = true;
		break;

	case SDL_MOUSEWHEEL:
		m_wheelMove.x = static_cast<float>(event.wheel.x);
		m_wheelMove.y = static_cast<float>(event.wheel.y);
		m_wheelMoved = true;
		break;
	}
}

void MouseInputContext::clearFrameEvents()
{
	m_moved = false;
	m_wheelMoved = false;
	m_wheelMove.x = 0.f;
	m_wheelMove.y = 0.f;
	m_justPressedButtons.fill(false);
	m_justReleasedButtons.fill(false);
	m_justDoubleClickedButtons.fill(false);
}

void MouseInputContext::clearAllEvents()
{
	m_pressedButtons.fill(false);
	clearFrameEvents();
}

bool MouseInputContext::isPressed(int button) const
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

bool MouseInputContext::isJustPressed(int button) const
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

bool MouseInputContext::isJustReleased(int button) const
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

bool MouseInputContext::isJustDoubleClicked(int button) const
{
	if (button < static_cast<int>(m_justDoubleClickedButtons.size()))
	{
		return m_justDoubleClickedButtons[button];
	}
	else
	{
		return false;
	}
}

void MouseInputContext::copyStateFrom(const MouseInputContext& from)
{
	m_pressedButtons = from.m_pressedButtons;
	m_position = from.m_position;
	m_wheelMove = from.m_wheelMove;
	m_moved = from.m_moved;
	m_wheelMoved = from.m_wheelMoved;
}

} // context
} // input
} // flat


