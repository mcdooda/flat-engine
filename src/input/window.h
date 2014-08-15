#ifndef FLAT_INPUT_WINDOW_H
#define FLAT_INPUT_WINDOW_H

#include <SDL2/SDL.h>
#include "../video/video.h"

namespace flat
{
namespace input
{

class Window
{
	public:
		Window(video::Window* videoWindow);
		~Window();
		
		inline bool isClosed() const { return m_closed; }
		inline bool isResized() const { return m_resized; }

		void clearEvents();
		void addEvent(const SDL_Event& e);
		
	private:
		bool m_closed;
		bool m_resized;
		
		video::Window* m_videoWindow;
};

} // input
} // flat

#endif // FLAT_INPUT_WINDOW_H


