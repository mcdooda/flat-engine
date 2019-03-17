#include "input/gamepads.h"
#include "input/context/inputcontext.h"

namespace flat
{
namespace input
{

Gamepads::Gamepads(const std::shared_ptr<context::InputContext>& globalInputContext) :
	m_globalInputContext(globalInputContext)
{
	
}

bool Gamepads::isPressed(GamepadIndex index, GamepadButton button) const
{
	const context::GamepadInputContext* gamepadInputContext = m_globalInputContext->getGamepadInputContext(index);
	if (gamepadInputContext != nullptr)
	{
		return gamepadInputContext->isPressed(button);
	}
	return false;
}

bool Gamepads::isJustPressed(GamepadIndex index, GamepadButton button) const
{
	const context::GamepadInputContext* gamepadInputContext = m_globalInputContext->getGamepadInputContext(index);
	if (gamepadInputContext != nullptr)
	{
		return gamepadInputContext->isJustPressed(button);
	}
	return false;
}

bool Gamepads::isJustReleased(GamepadIndex index, GamepadButton button) const
{
	const context::GamepadInputContext* gamepadInputContext = m_globalInputContext->getGamepadInputContext(index);
	if (gamepadInputContext != nullptr)
	{
		return gamepadInputContext->isJustReleased(button);
	}
	return false;
}

bool Gamepads::hasAxisJustMoved(GamepadIndex index, GamepadAxis axis) const
{
	const context::GamepadInputContext* gamepadInputContext = m_globalInputContext->getGamepadInputContext(index);
	if (gamepadInputContext != nullptr)
	{
		return gamepadInputContext->hasAxisJustMoved(axis);
	}
	return false;
}

float Gamepads::getAxisValue(GamepadIndex index, GamepadAxis axis) const
{
	const context::GamepadInputContext* gamepadInputContext = m_globalInputContext->getGamepadInputContext(index);
	if (gamepadInputContext != nullptr)
	{
		return gamepadInputContext->getAxisValue(axis);
	}
	return false;
}

flat::Vector2 Gamepads::getLeftStickValue(GamepadIndex index) const
{
	const context::GamepadInputContext* gamepadInputContext = m_globalInputContext->getGamepadInputContext(index);
	if (gamepadInputContext != nullptr)
	{
		return gamepadInputContext->getLeftStickValue();
	}
	return flat::Vector2(0.f, 0.f);
}

flat::Vector2 Gamepads::getRightStickValue(GamepadIndex index) const
{
	const context::GamepadInputContext* gamepadInputContext = m_globalInputContext->getGamepadInputContext(index);
	if (gamepadInputContext != nullptr)
	{
		return gamepadInputContext->getRightStickValue();
	}
	return flat::Vector2(0.f, 0.f);
}

} // input
} // flat


