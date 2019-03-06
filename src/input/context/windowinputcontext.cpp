#include "input/context/windowinputcontext.h"

#include "video/window.h"

namespace flat
{
namespace input
{
namespace context
{

WindowInputContext::WindowInputContext(video::Window* videoWindow) :
	m_videoWindow(videoWindow)
{
	clearAllEvents();
}

bool WindowInputContext::addEvent(const SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_QUIT:
		m_closed = true;
		return true;

	case SDL_WINDOWEVENT:
		switch (event.window.event)
		{
		case SDL_WINDOWEVENT_RESIZED:
			m_resized = true;
			m_videoWindow->resized({ event.window.data1, event.window.data2 });
			return true;
		}
		break;
	}

	return false;
}

void WindowInputContext::clearFrameEvents()
{
	m_closed = false;
	m_resized = false;
}

void WindowInputContext::clearAllEvents()
{
	clearFrameEvents();
}

void WindowInputContext::copyStateFrom(const WindowInputContext& from)
{
	m_closed = from.m_closed;
	m_resized = from.m_resized;
}

} // context
} // input
} // flat


