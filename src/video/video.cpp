#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "video.h"
#include "font/font.h"
#include "../memory/memory.h"

namespace flat
{
namespace video
{

Video::Video()
{
	window = new Window;
	Texture::open();
	font::Font::open();
}

Video::~Video()
{
	font::Font::close();
	Texture::close();
	FLAT_DELETE(window);
}

void Video::endFrame()
{
	window->endFrame();
}

void Video::clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Video::setClearColor(const Color& color)
{
	glClearColor(color.getR(), color.getG(), color.getB(), color.getA());
}

} // video
} // flat


