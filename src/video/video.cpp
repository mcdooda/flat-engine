#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "video.h"
#include "text.h"

namespace flat
{
namespace video
{

Video::Video()
{
	Text::open();
	window = new Window;
}

Video::~Video()
{
	Text::close();
	delete window;
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


