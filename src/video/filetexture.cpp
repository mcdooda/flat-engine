#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#include "filetexture.h"

namespace flat
{
namespace video
{

FileTexture::FileTexture(const std::string& fileName) :
	m_fileName(fileName)
{
	m_surface = IMG_Load(fileName.c_str());
	
	if (m_surface != nullptr)
		load();
		
	else
		std::cerr << "Warning: error in IMG_Load(" << fileName.c_str() << ") : " << IMG_GetError() << std::endl;
}

FileTexture::FileTexture()
{
	
}

FileTexture::~FileTexture()
{
	free();
}

void FileTexture::getPixel(const Vector2& pixelPosition, Color& color) const
{
	int x = static_cast<int>(std::floor(pixelPosition.x));
	int y = static_cast<int>(std::ceil(pixelPosition.y));
	FLAT_ASSERT(x >= 0 && x <= m_surface->w);
	FLAT_ASSERT(y >= 0 && y <= m_surface->h);

	int pixelIndex = (m_surface->h - y) * m_surface->w + x;
	std::uint32_t pixel = *(static_cast<std::uint32_t*>(m_surface->pixels) + pixelIndex);
	std::uint8_t r, g, b, a;
	SDL_GetRGBA(pixel, m_surface->format, &r, &g, &b, &a);
	color = Color(r, g, b, a);
}

void FileTexture::load()
{
	m_size = Vector2(static_cast<float>(m_surface->w), static_cast<float>(m_surface->h));
	
	glGenTextures(1, &m_textureId);
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_surface->w, m_surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void FileTexture::free()
{
	glDeleteTextures(1, &m_textureId);
	SDL_FreeSurface(m_surface);
	m_surface = nullptr;
}

} // video
} // flat


