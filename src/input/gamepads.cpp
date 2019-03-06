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
	return m_globalInputContext->getGamepadInputContext(index).isPressed(button);
}

bool Gamepads::isJustPressed(GamepadIndex index, GamepadButton button) const
{
	return m_globalInputContext->getGamepadInputContext(index).isJustPressed(button);
}

bool Gamepads::isJustReleased(GamepadIndex index, GamepadButton button) const
{
	return m_globalInputContext->getGamepadInputContext(index).isJustReleased(button);
}

bool Gamepads::hasAxisJustMoved(GamepadIndex index, GamepadAxis axis) const
{
	return m_globalInputContext->getGamepadInputContext(index).hasAxisJustMoved(axis);
}

float Gamepads::getAxisValue(GamepadIndex index, GamepadAxis axis) const
{
	return m_globalInputContext->getGamepadInputContext(index).getAxisValue(axis);
}

flat::Vector2 Gamepads::getLeftStickValue(GamepadIndex index) const
{
	return m_globalInputContext->getGamepadInputContext(index).getLeftStickValue();
}

flat::Vector2 Gamepads::getRightStickValue(GamepadIndex index) const
{
	return m_globalInputContext->getGamepadInputContext(index).getRightStickValue();
}

} // input
} // flat


