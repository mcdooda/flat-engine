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
	return (buttonState & SDL_BUTTON(button)) != 0;
}

bool Mouse::isJustPressed(int button) const
{
	if (button < m_justPressedButtons.size())
		return m_justPressedButtons[button];

	return false;
}

bool Mouse::isJustReleased(int button) const
{
	if (button < m_justPressedButtons.size())
		return m_justReleasedButtons[button];

	return false;
}

void Mouse::clearEvents()
{
	m_moved = false;
	m_wheelMoved = false;
	m_wheelMove.x = 0.f;
	m_wheelMove.y = 0.f;
	m_justPressedButtons.fill(false);
	m_justReleasedButtons.fill(false);
}

void Mouse::addEvent(const SDL_Event& e)
{
	switch (e.type)
	{
		case SDL_MOUSEBUTTONDOWN:
		m_position.x = static_cast<float>(e.button.x);
		m_position.y = m_videoWindow->getSize().y - e.button.y;
		m_justPressedButtons[e.button.button] = true;
		break;

		case SDL_MOUSEBUTTONUP:
		m_position.x = static_cast<float>(e.button.x);
		m_position.y = m_videoWindow->getSize().y - e.button.y;
		m_justReleasedButtons[e.button.button] = true;
		break;

		case SDL_MOUSEMOTION:
		m_position.x = static_cast<float>(e.button.x);
		m_position.y = m_videoWindow->getSize().y - e.button.y;
		m_moved = true;
		break;
		
		case SDL_MOUSEWHEEL:
		m_wheelMove.x = static_cast<float>(e.wheel.x);
		m_wheelMove.y = static_cast<float>(e.wheel.y);
		m_wheelMoved = true;
		break;
	}
}

} // input
} // flat


