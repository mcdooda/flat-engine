#include "mouse.h"

namespace flat
{
namespace input
{

Mouse::Mouse(video::Window* videoWindow) :
	m_videoWindow(videoWindow)
{
	clearEvents();
}

Mouse::~Mouse()
{

}

bool Mouse::isPressed(int button) const
{
	Uint8 buttonState = SDL_GetMouseState(nullptr, nullptr);
	return buttonState & SDL_BUTTON(button);
}

bool Mouse::isJustPressed(int button) const
{
	if (m_justPressedButtons.isInRange(button))
		return m_justPressedButtons[button];

	return false;
}

bool Mouse::isJustReleased(int button) const
{
	if (m_justPressedButtons.isInRange(button))
		return m_justReleasedButtons[button];

	return false;
}

void Mouse::clearEvents()
{
	m_moved = false;
	m_wheelMoved = false;
	m_wheelMove.x = 0.f;
	m_wheelMove.y = 0.f;
	m_justPressedButtons.memset(0);
	m_justReleasedButtons.memset(0);
}

void Mouse::addEvent(const SDL_Event& e)
{
	switch (e.type)
	{
		case SDL_MOUSEBUTTONDOWN:
		m_position.x = e.button.x;
		m_position.y = m_videoWindow->getSize().y - e.button.y;
		m_justPressedButtons[e.button.button] = true;
		break;

		case SDL_MOUSEBUTTONUP:
		m_position.x = e.button.x;
		m_position.y = m_videoWindow->getSize().y - e.button.y;
		m_justReleasedButtons[e.button.button] = true;
		break;

		case SDL_MOUSEMOTION:
		m_position.x = e.button.x;
		m_position.y = m_videoWindow->getSize().y - e.button.y;
		m_moved = true;
		break;
		
		case SDL_MOUSEWHEEL:
		m_wheelMove.x = e.wheel.x;
		m_wheelMove.y = e.wheel.y;
		m_wheelMoved = true;
		break;
	}
}

} // input
} // flat


