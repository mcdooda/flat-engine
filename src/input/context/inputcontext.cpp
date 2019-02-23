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
	initGamepads();
}

void InputContext::addEvent(const SDL_Event& event)
{
	if (m_keyboardInputContext.addEvent(event))
		return;

	if (m_mouseInputContext.addEvent(event))
		return;

	if (m_windowInputContext.addEvent(event))
		return;

	for (GamepadInputContext& gamepadInputContext : m_gamepadInputContexts)
	{
		if (gamepadInputContext.addEvent(event))
			return;
	}

	if (handleDeviceEvent(event))
		return;
}

void InputContext::clearFrameEvents()
{
	m_keyboardInputContext.clearFrameEvents();
	m_mouseInputContext.clearFrameEvents();
	m_windowInputContext.clearFrameEvents();
	for (GamepadInputContext& gamepadInputContext : m_gamepadInputContexts)
	{
		gamepadInputContext.clearFrameEvents();
	}
}

void InputContext::clearAllEvents()
{
	m_keyboardInputContext.clearAllEvents();
	m_mouseInputContext.clearAllEvents();
	m_windowInputContext.clearAllEvents();
	for (GamepadInputContext& gamepadInputContext : m_gamepadInputContexts)
	{
		gamepadInputContext.clearAllEvents();
	}
}

void InputContext::copyStateFrom(const InputContext& from)
{
	m_keyboardInputContext.copyStateFrom(from.m_keyboardInputContext);
	m_mouseInputContext.copyStateFrom(from.m_mouseInputContext);
	m_windowInputContext.copyStateFrom(from.m_windowInputContext);
	FLAT_ASSERT(m_gamepadInputContexts.size() == from.m_gamepadInputContexts.size());
	for (size_t i = 0; i < m_gamepadInputContexts.size(); ++i)
	{
		m_gamepadInputContexts[i].copyStateFrom(from.m_gamepadInputContexts[i]);
	}
}


void InputContext::initGamepads()
{
	for (int i = 0; i < SDL_NumJoysticks(); ++i)
	{
		if (SDL_IsGameController(i))
		{
			SDL_GameController* controller = SDL_GameControllerOpen(i);
			if (controller != nullptr)
			{
				SDL_Joystick* joystick = SDL_JoystickOpen(i);
				SDL_JoystickID joystickID = SDL_JoystickInstanceID(joystick);
				m_gamepadInputContexts.emplace_back(joystickID);
				std::cout << "Controller " << i << " added" << std::endl;
			}
		}
	}

}

bool InputContext::handleDeviceEvent(const SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_CONTROLLERDEVICEADDED:
	{
		SDL_Joystick* joystick = SDL_JoystickOpen(event.cdevice.which);
		SDL_JoystickID joystickID = SDL_JoystickInstanceID(joystick);
		m_gamepadInputContexts.emplace_back(joystickID);
		std::cout << "Controller " << event.cdevice.which << " added" << std::endl;
		return true;
	}

	case SDL_CONTROLLERDEVICEREMOVED:
		std::cout << "Controller " << event.cdevice.which << " removed" << std::endl;
		FLAT_ASSERT_MSG(false, "not implemented");
		return true;

	case SDL_CONTROLLERDEVICEREMAPPED:
		std::cout << "Controller " << event.cdevice.which << " remapped" << std::endl;
		FLAT_ASSERT_MSG(false, "not implemented");
		return true;
	}
	return false;
}

} // context
} // input
} // flat


