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

bool Mouse::isPressed(int button)
{
	Uint8 buttonState = SDL_GetMouseState(NULL, NULL);
	return buttonState & SDL_BUTTON(button);
}

bool Mouse::isJustPressed(int button)
{
	if (button < NUM_BUTTONS)
		return m_justPressedButtons[button];

	return false;
}

bool Mouse::isJustReleased(int button)
{
	if (button < NUM_BUTTONS)
		return m_justReleasedButtons[button];

	return false;
}

void Mouse::clearEvents()
{
	m_moved = false;
	memset(m_justPressedButtons, 0, NUM_BUTTONS * sizeof(bool));
	memset(m_justReleasedButtons, 0, NUM_BUTTONS * sizeof(bool));
}

void Mouse::addEvent(const SDL_Event& e)
{
	switch (e.type)
	{
		case SDL_MOUSEBUTTONDOWN:
		m_position.setX(e.button.x);
		m_position.setY(m_videoWindow->getSize().getY() - e.button.y);

		if (e.button.button < NUM_BUTTONS)
			m_justPressedButtons[e.button.button] = true;

		break;

		case SDL_MOUSEBUTTONUP:
		m_position.setX(e.button.x);
		m_position.setY(m_videoWindow->getSize().getY() - e.button.y);

		if (e.button.button < NUM_BUTTONS)
			m_justReleasedButtons[e.button.button] = true;

		break;

		case SDL_MOUSEMOTION:
		m_position.setX(e.button.x);
		m_position.setY(m_videoWindow->getSize().getY() - e.button.y);
		m_moved = true;
		break;
	}
}

} // input
} // flat


