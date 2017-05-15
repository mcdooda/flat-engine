#include "inputcontext.h"

#include "../../flat.h"

namespace flat
{
namespace input
{
namespace context
{

InputContext::InputContext(Flat& flat) :
	m_mouseInputContext(flat.video->window),
	m_windowInputContext(flat.video->window)
{
}

void InputContext::addEvent(const SDL_Event& event)
{
	m_keyboardInputContext.addEvent(event);
	m_mouseInputContext.addEvent(event);
	m_windowInputContext.addEvent(event);
}

void InputContext::clearEvents()
{
	m_keyboardInputContext.clearEvents();
	m_mouseInputContext.clearEvents();
	m_windowInputContext.clearEvents();
}

} // context
} // input
} // flat


