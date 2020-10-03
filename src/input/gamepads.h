#ifndef FLAT_INPUT_GAMEPADS_H
#define FLAT_INPUT_GAMEPADS_H

#include <vector>
#include <memory>

#include "input/gamepadbuttons.h"

#include "misc/vector.h"

namespace flat
{
namespace input
{
namespace context
{
class InputContext;
}

class Gamepads
{
	public:
		Gamepads() = delete;
		Gamepads(const Gamepads&) = delete;
		Gamepads(Gamepads&&) = delete;
		Gamepads(const std::shared_ptr<context::InputContext>& globalInputContext);
		~Gamepads() = default;

		bool isPressed(GamepadIndex index, GamepadButton button) const;
		bool isJustPressed(GamepadIndex index, GamepadButton button) const;
		bool isJustReleased(GamepadIndex index, GamepadButton button) const;

		bool hasAxisJustMoved(GamepadIndex index, GamepadAxis axis) const;
		float getAxisValue(GamepadIndex index, GamepadAxis axis) const;

		template<typename T>
		void triggerPressedButtons(GamepadIndex index, T callback)
		{
			context::GamepadInputContext* gamepadInputContext = m_globalInputContext->getGamepadInputContext(index);
			if (gamepadInputContext != nullptr)
			{
				gamepadInputContext->triggerPressedButtons(callback);
			}
		}

		template<typename T>
		void triggerReleasedButtons(GamepadIndex index, T callback)
		{
			context::GamepadInputContext* gamepadInputContext = m_globalInputContext->getGamepadInputContext(index);
			if (gamepadInputContext != nullptr)
			{
				gamepadInputContext->triggerReleasedButtons(callback);
			}
		}

		flat::Vector2 getLeftStickValue(GamepadIndex index) const;
		flat::Vector2 getRightStickValue(GamepadIndex index) const;

	private:
		std::shared_ptr<context::InputContext> m_globalInputContext;
};

} // input
} // flat

#endif // FLAT_INPUT_GAMEPADS_H


