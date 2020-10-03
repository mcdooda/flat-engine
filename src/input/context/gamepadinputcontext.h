#ifndef FLAT_INPUT_CONTEXT_GAMEPADINPUTCONTEXT_H
#define FLAT_INPUT_CONTEXT_GAMEPADINPUTCONTEXT_H

#include <array>
#include <SDL2/SDL.h>

#include "input/gamepadbuttons.h"

#include "misc/vector.h"

namespace flat
{
namespace input
{
namespace context
{

class GamepadInputContext
{
	public:
		static constexpr int NUM_BUTTONS = SDL_CONTROLLER_BUTTON_MAX;
		static constexpr int NUM_AXES = SDL_CONTROLLER_AXIS_MAX;

	public:
		GamepadInputContext() = delete;
		GamepadInputContext(const GamepadInputContext&) = default;
		GamepadInputContext(GamepadInputContext&&) = default;
		GamepadInputContext(SDL_JoystickID joystickId);
		~GamepadInputContext() = default;
		GamepadInputContext& operator=(const GamepadInputContext&) = default;

		bool addEvent(const SDL_Event& event);
		void clearFrameEvents();
		void clearAllEvents();

		bool isPressed(GamepadButton button) const;
		bool isJustPressed(GamepadButton button) const;
		bool isJustReleased(GamepadButton button) const;

		bool hasAxisJustMoved(GamepadAxis axis) const;
		float getAxisValue(GamepadAxis axis) const;

		flat::Vector2 getLeftStickValue() const;
		flat::Vector2 getRightStickValue() const;

		void copyStateFrom(const GamepadInputContext& from);

		template<typename T>
		void triggerPressedButtons(T callback)
		{
			for (size_t i = 0; i < m_justPressedButtons.size(); i++)
			{
				if (m_justPressedButtons[i]) {
					callback(static_cast<SDL_GameControllerButton>(i));
				}
			}
		}

		template<typename T>
		void triggerReleasedButtons(T callback)
		{
			for (size_t i = 0; i < m_justReleasedButtons.size(); i++)
			{
				if (m_justReleasedButtons[i]) {
					callback(static_cast<SDL_GameControllerButton>(i));
				}
			}
		}

	private:
		static void sanitizeStickValue(flat::Vector2& stickValue);

	private:
		SDL_JoystickID m_joystickId;
		std::array<bool, NUM_BUTTONS> m_pressedButtons;
		std::array<bool, NUM_BUTTONS> m_justPressedButtons;
		std::array<bool, NUM_BUTTONS> m_justReleasedButtons;

		std::array<float, NUM_AXES> m_axes;
		std::array<bool, NUM_AXES> m_justMovedAxes;
};

} // context
} // input
} // flat

#endif // FLAT_INPUT_CONTEXT_GAMEPADINPUTCONTEXT_H


