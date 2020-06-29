#include <SDL2/SDL.h>

#include "input/context/mouseinputcontext.h"

#include "video/window.h"

namespace flat
{
namespace input
{
namespace context
{

MouseInputContext::MouseInputContext(video::Window* videoWindow) :
	m_videoWindow(videoWindow)
{
	clearAllEvents();
}

static constexpr int doubleClickThreeshold = 200;

bool MouseInputContext::addEvent(const SDL_Event& event)
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
			const float squaredManhattan = (m_position.x - m_lastClickPosition.x) * (m_position.x - m_lastClickPosition.x)
									 + (m_position.y - m_lastClickPosition.y) * (m_position.y - m_lastClickPosition.y);
			if (squaredManhattan < doubleClickThreeshold) {
				m_justDoubleClickedButtons[event.button.button] = true;
			}
		}
		m_lastClickPosition = m_position;
		return true;
	case SDL_MOUSEBUTTONUP:
		m_position.x = static_cast<float>(event.button.x);
		m_position.y = m_videoWindow->getSize().y - event.button.y;
		m_pressedButtons[event.button.button] = false;
		m_justReleasedButtons[event.button.button] = true;
		return true;

	case SDL_MOUSEMOTION:
		m_position.x = static_cast<float>(event.button.x);
		m_position.y = m_videoWindow->getSize().y - event.button.y;
		m_moved = true;
		return true;

	case SDL_MOUSEWHEEL:
		m_wheelMove.x = static_cast<float>(event.wheel.x);
		m_wheelMove.y = static_cast<float>(event.wheel.y);
		m_wheelMoved = true;
		return true;
	}

	return false;
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
	return false;
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


