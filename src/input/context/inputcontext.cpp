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
	m_windowInputContext(flat.video->window),
	m_isEnabled(false)
{
}

void InputContext::addEvent(const SDL_Event& event)
{
	m_keyboardInputContext.addEvent(event);
	m_mouseInputContext.addEvent(event);
	m_windowInputContext.addEvent(event);
}

void InputContext::clearFrameEvents()
{
	m_keyboardInputContext.clearFrameEvents();
	m_mouseInputContext.clearFrameEvents();
	m_windowInputContext.clearFrameEvents();
}

void InputContext::clearAllEvents()
{
	m_keyboardInputContext.clearAllEvents();
	m_mouseInputContext.clearAllEvents();
	m_windowInputContext.clearAllEvents();
}

void InputContext::copyStateFrom(const InputContext& from)
{
	m_keyboardInputContext.copyStateFrom(from.m_keyboardInputContext);
	m_mouseInputContext.copyStateFrom(from.m_mouseInputContext);
	m_windowInputContext.copyStateFrom(from.m_windowInputContext);
}

} // context
} // input
} // flat


