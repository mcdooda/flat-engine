#ifndef FLAT_INPUT_MOUSE_H
#define FLAT_INPUT_MOUSE_H

#include <SDL2/SDL.h>
#include "../containers/fixedsizearray.h"
#include "../geometry/vector2.h"
#include "../video/video.h"
#include "../video/view.h"

#define M(m) SDL_BUTTON_##m
#define NUM_BUTTONS 8

namespace flat
{
namespace input
{

class Mouse
{
	public:
		Mouse(video::Window* videoWindow);
		~Mouse();
		
		bool isPressed(int button);
		bool isJustPressed(int button);
		bool isJustReleased(int button);

		inline bool justMoved() const { return m_moved; }

		inline const geometry::Vector2& getPosition() const { return m_position; }

		void clearEvents();
		void addEvent(const SDL_Event& e);
		
	private:
		containers::FixedSizeArray<bool, NUM_BUTTONS> m_justPressedButtons;
		containers::FixedSizeArray<bool, NUM_BUTTONS> m_justReleasedButtons;
		
		video::Window* const m_videoWindow;

		geometry::Vector2 m_position;
		bool m_moved;
};

} // input
} // flat

#endif // FLAT_INPUT_MOUSE_H


