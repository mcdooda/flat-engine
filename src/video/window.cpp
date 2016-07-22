#include <iostream>
#include <GL/glew.h>
#include <flat.h>
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
	if (vsync) // TODO vsync
		rendererFlags |= SDL_RENDERER_PRESENTVSYNC;

	SDL_GLContext glContext = SDL_GL_CreateContext(m_window);
	if (glContext == nullptr)
	{
		printf("There was an error creating the OpenGL context!\n");
		FLAT_BREAK();
		exit(1);
	}

	SDL_GL_MakeCurrent(m_window, glContext);
	
	if (!glOk)
	{
		glOk = true;
		initGlew();
		initGL();
	}

	m_fullScreen = fullScreen;
	m_vsync = vsync;

	initSize(size);
}

void Window::setTitle(const std::string& title)
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

const geometry::Vector2& Window::getDesktopSize() const
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
	SDL_GL_SwapWindow(m_window);
}

bool Window::supportsGlExtensions()
{
	return GLEW_ARB_vertex_program
	    && GLEW_ARB_fragment_program
	    && GLEW_ARB_texture_float
	    && GLEW_ARB_draw_buffers
	    && GLEW_ARB_framebuffer_object;
}

void Window::initSize(const geometry::Vector2& size)
{
	resized(size);
}

void Window::initGlew()
{
	glewExperimental = GL_TRUE;
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	int err = glewInit();
	if (err != GLEW_OK)
	{
		std::cerr << "Fatal: glewInit failed: " << glewGetErrorString(err) << std::endl;
		FLAT_BREAK();
		exit(1);
	}

	if (!supportsGlExtensions())
	{
		std::cerr << "Fatal: Shaders not supported!" << std::endl;
		FLAT_BREAK();
		exit(1);
	}
}

void Window::initGL()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

} // video
} // flat


