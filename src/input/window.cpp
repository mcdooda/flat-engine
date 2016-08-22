#include "window.h"
#include "../video/video.h"

namespace flat
{
namespace input
{

Window::Window(video::Window* videoWindow) :
	m_videoWindow(videoWindow)
{
	clearEvents();
}

Window::~Window()
{
	
}

void Window::clearEvents()
{
	m_closed = false;
	m_resized = false;
}

void Window::addEvent(const SDL_Event& e)
{
	switch (e.type)
	{
		case SDL_QUIT:
		m_closed = true;
		break;

		case SDL_WINDOWEVENT:
		switch (e.window.event)
		{
			case SDL_WINDOWEVENT_RESIZED:
			m_resized = true;
			m_videoWindow->resized(Vector2(e.window.data1, e.window.data2));
			break;
		}
		break;
	}
}

} // input
} // flat


