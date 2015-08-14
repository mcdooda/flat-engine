#ifndef FLAT_VIDEO_WINDOW_H
#define FLAT_VIDEO_WINDOW_H

#include <string>
#include <SDL2/SDL.h>
#include "../geometry/vector2.h"

namespace flat
{
namespace video
{

class Window
{
	public:
		Window();
		~Window();
		
		void open(const geometry::Vector2& size, bool fullScreen, bool vsync);
		void setTitle(std::string title);

		void toggleFullScreen();
		
		void showCursor() const;
		void hideCursor() const;
		
		inline bool isFullScreen() const { return m_fullScreen; }
		inline bool isVsyncEnabled() const { return m_vsync; }

		inline const geometry::Vector2& getSize() { return m_size; }
		void resized(const geometry::Vector2& size);

		geometry::Vector2 getDesktopSize();
		
		void endFrame();
		
		static bool supportsGlExtensions();
		
	private:
		void initSize(const geometry::Vector2& size);
		
		static void initGlew();
		static void initGL();
	
	private:
		geometry::Vector2 m_size;
		geometry::Vector2 m_oldSize;
		bool m_fullScreen;
		bool m_vsync;
		SDL_Window* m_window;
		SDL_Renderer* m_renderer;
		
		static bool glOk;
};

} // video
} // flat

#endif // FLAT_VIDEO_WINDOW_H


