#ifndef FLAT_INPUT_CONTEXT_WINDOWINPUTCONTEXT_H
#define FLAT_INPUT_CONTEXT_WINDOWINPUTCONTEXT_H

#include <SDL2/SDL.h>

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

class WindowInputContext
{
	public:
		WindowInputContext() = delete;
		WindowInputContext(const WindowInputContext&) = delete;
		WindowInputContext(WindowInputContext&&) = delete;
		WindowInputContext(video::Window* videoWindow);
		~WindowInputContext() = default;
		WindowInputContext& operator=(const WindowInputContext&) = default;

		void addEvent(const SDL_Event& event);
		void clearFrameEvents();
		void clearAllEvents();

		inline bool isClosed() const { return m_closed; }
		inline bool isResized() const { return m_resized; }

	private:
		video::Window* m_videoWindow;

		bool m_closed : 1;
		bool m_resized : 1;
};

} // context
} // input
} // flat

#endif // FLAT_INPUT_CONTEXT_WINDOWINPUTCONTEXT_H


