#ifndef FLAT_INPUT_MOUSE_H
#define FLAT_INPUT_MOUSE_H

#include <array>
#include <SDL2/SDL.h>
#include "../misc/vector.h"
#include "../video/video.h"
#include "../video/view.h"

#define M(m) SDL_BUTTON_##m
#define NUM_BUTTONS 255

namespace flat
{
namespace input
{

class Mouse
{
	public:
		Mouse(video::Window* videoWindow);
		~Mouse();
		
		bool isPressed(int button) const;
		bool isJustPressed(int button) const;
		bool isJustReleased(int button) const;

		inline bool justMoved() const { return m_moved; }
		inline const Vector2& getPosition() const { return m_position; }
		
		inline bool wheelJustMoved() const { return m_wheelMoved; }
		inline const Vector2& getWheelMove() const { return m_wheelMove; }

		void clearEvents();
		void addEvent(const SDL_Event& e);
		
	private:
		std::array<bool, NUM_BUTTONS> m_justPressedButtons;
		std::array<bool, NUM_BUTTONS> m_justReleasedButtons;
		
		video::Window* const m_videoWindow;

		Vector2 m_position;
		Vector2 m_wheelMove;
		bool m_moved : 1;
		bool m_wheelMoved : 1;
};

} // input
} // flat

#endif // FLAT_INPUT_MOUSE_H


