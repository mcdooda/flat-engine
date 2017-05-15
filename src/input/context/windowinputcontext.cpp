#include "windowinputcontext.h"

#include "../../video/window.h"

namespace flat
{
namespace input
{
namespace context
{

WindowInputContext::WindowInputContext(video::Window* videoWindow) :
	m_videoWindow(videoWindow)
{

}

void WindowInputContext::addEvent(const SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_QUIT:
		m_closed = true;
		break;

	case SDL_WINDOWEVENT:
		switch (event.window.event)
		{
		case SDL_WINDOWEVENT_RESIZED:
			m_resized = true;
			m_videoWindow->resized({ event.window.data1, event.window.data2 });
			break;
		}
		break;
	}
}

void WindowInputContext::clearEvents()
{
	m_closed = false;
	m_resized = false;
}

} // context
} // input
} // flat


