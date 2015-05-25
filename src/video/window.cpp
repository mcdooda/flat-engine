#include <iostream>
#include <GL/glew.h>
#include "window.h"

namespace flat
{
namespace video
{

bool Window::glOk = false;

Window::Window()
{
	
}

Window::~Window()
{
	
}

void Window::open(const geometry::Vector2& size, bool fullScreen, bool vsync)
{
	int windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
	
	if (fullScreen)
		windowFlags |= SDL_WINDOW_FULLSCREEN;
	
	m_window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, size.x, size.y, windowFlags);
	
	int rendererFlags = 0;
	
	if (vsync)
		rendererFlags |= SDL_RENDERER_PRESENTVSYNC;
	
	m_renderer = SDL_CreateRenderer(m_window, -1, rendererFlags);
	
	if (!glOk)
	{
		glOk = true;
		initGlew();
		initGL();
	}
	
	SDL_WarpMouseInWindow(m_window, size.x / 2, size.y / 2);

	m_fullScreen = fullScreen;
	m_vsync = vsync;

	initSize(size);
}

void Window::setTitle(std::string title)
{
	SDL_SetWindowTitle(m_window, title.c_str());
}

void Window::toggleFullScreen()
{
	m_fullScreen = !m_fullScreen;
	geometry::Vector2 desktopSize = getDesktopSize();
	if (m_fullScreen)
	{
		SDL_SetWindowSize(m_window, desktopSize.x, desktopSize.y);
		SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
	}
	else
	{
		SDL_SetWindowFullscreen(m_window, 0);
		SDL_SetWindowSize(m_window, desktopSize.x / 2, desktopSize.y / 2);
	}
}

void Window::showCursor() const
{
	SDL_ShowCursor(1);
}

void Window::hideCursor() const
{
	SDL_ShowCursor(0);
}

void Window::resized(const geometry::Vector2& size)
{
	m_size = size;
}

geometry::Vector2 Window::getDesktopSize()
{
	static geometry::Vector2 desktopSize;
	if (desktopSize.x == 0)
	{
		SDL_DisplayMode desktopDisplayMode;
		SDL_GetDesktopDisplayMode(0, &desktopDisplayMode);
		desktopSize = geometry::Vector2(desktopDisplayMode.w, desktopDisplayMode.h);
	}
	return desktopSize;
}

void Window::endFrame()
{
	glUseProgram(0);
	glFlush();
	SDL_RenderPresent(m_renderer);
}

void Window::initSize(const geometry::Vector2& size)
{
	resized(size);
}

void Window::initGlew()
{
	int err = glewInit();
	if (err != GLEW_OK)
	{
		std::cerr << "Fatal: glewInit failed: " << glewGetErrorString(err) << std::endl;
		exit(1);
	}

	if (!GLEW_ARB_vertex_program
	 || !GLEW_ARB_fragment_program
	 || !GLEW_ARB_texture_float
	 || !GLEW_ARB_draw_buffers
	 || !GLEW_ARB_framebuffer_object)
		std::cerr << "Warning: Shaders not supported!" << std::endl;
}

void Window::initGL()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

} // video
} // flat


