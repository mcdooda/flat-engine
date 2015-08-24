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
	window = new Window;
	Texture::open();
	Text::open();
}

Video::~Video()
{
	Text::close();
	Texture::close();
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

std::shared_ptr<const FileTexture> Video::getTexture(const std::string& fileName)
{
	std::unordered_map<std::string, std::weak_ptr<const FileTexture>>::iterator it = m_loadedTextures.find(fileName);
	if (it != m_loadedTextures.end())
		if (std::shared_ptr<const FileTexture> sharedTexture = it->second.lock())
			return sharedTexture;
	
	const FileTexture* texture = new FileTexture(fileName);
	std::shared_ptr<const FileTexture> sharedTexture;
	sharedTexture.reset(texture);
	m_loadedTextures[fileName] = std::weak_ptr<const FileTexture>(sharedTexture);
	return sharedTexture;
}

} // video
} // flat


