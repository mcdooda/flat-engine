#ifndef FLAT_VIDEO_WINDOW_H
#define FLAT_VIDEO_WINDOW_H

#include <string>
#include <SDL2/SDL.h>
#include "../misc/vector.h"

namespace flat
{
namespace video
{

class Window
{
	public:
		Window();
		~Window();
		
		void open(const Vector2& size, bool fullScreen, bool vsync);
		void setTitle(const std::string& title);

		void toggleFullScreen();
		
		void showCursor() const;
		void hideCursor() const;
		
		inline bool isFullScreen() const { return m_fullScreen; }
		inline bool isVsyncEnabled() const { return m_vsync; }

		inline const Vector2& getSize() const { return m_size; }
		void resized(const Vector2& size);

		const Vector2& getDesktopSize() const;
		
		void endFrame();
		
		static bool supportsGlExtensions();
		
	private:
		void initSize(const Vector2& size);
		
		static void initGlew();
		static void initGL();
	
	private:
		Vector2 m_size;
		Vector2 m_oldSize;
		bool m_fullScreen;
		bool m_vsync;
		SDL_Window* m_window;
		SDL_Renderer* m_renderer;
		
		static bool glOk;
};

} // video
} // flat

#endif // FLAT_VIDEO_WINDOW_H


