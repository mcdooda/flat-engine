#ifndef FLAT_INPUT_CONTEXT_MOUSEINPUTCONTEXT_H
#define FLAT_INPUT_CONTEXT_MOUSEINPUTCONTEXT_H

#include <array>

#include "misc/vector.h"

union SDL_Event;

namespace flat
{
namespace video
{
class Window;
}
namespace input
{
namespace context
{

class MouseInputContext
{
	public:
		static constexpr int NUM_BUTTONS = 255;

	public:
		MouseInputContext() = delete;
		MouseInputContext(const MouseInputContext&) = delete;
		MouseInputContext(MouseInputContext&&) = delete;
		MouseInputContext(video::Window* videoWindow);
		~MouseInputContext() = default;
		MouseInputContext& operator=(const MouseInputContext&) = default;

		bool addEvent(const SDL_Event& event);
		void clearFrameEvents();
		void clearAllEvents();

		bool isPressed(int button) const;
		bool isJustPressed(int button) const;
		bool isJustReleased(int button) const;
		bool isJustDoubleClicked(int button) const;

		inline bool justMoved() const { return m_moved; }
		inline const Vector2& getPosition() const { return m_position; }

		inline bool wheelJustMoved() const { return m_wheelMoved; }
		inline const Vector2& getWheelMove() const { return m_wheelMove; }

		void copyStateFrom(const MouseInputContext& from);

	private:
		std::array<bool, NUM_BUTTONS> m_pressedButtons;
		std::array<bool, NUM_BUTTONS> m_justPressedButtons;
		std::array<bool, NUM_BUTTONS> m_justReleasedButtons;
		std::array<bool, NUM_BUTTONS> m_justDoubleClickedButtons;

		const video::Window* m_videoWindow;

		Vector2 m_position;
		Vector2 m_wheelMove;
		Vector2 m_lastClickPosition;
		bool m_moved : 1;
		bool m_wheelMoved : 1;
};

} // context
} // input
} // flat

#endif // FLAT_INPUT_CONTEXT_MOUSEINPUTCONTEXT_H


