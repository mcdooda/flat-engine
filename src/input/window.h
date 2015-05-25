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
		video::Window* m_videoWindow;
		
		bool m_closed;
		bool m_resized;
};

} // input
} // flat

#endif // FLAT_INPUT_WINDOW_H


