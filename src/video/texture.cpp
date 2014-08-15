#include "texture.h"

namespace engine
{
namespace video
{

Texture::Texture() :
	m_textureId(0)
{
	
}

Texture::Texture(GLuint textureId, const geometry::Vector2& size, const std::string& name) :
	m_textureId(textureId),
	m_size(size),
	m_name(name)
{
	
}

Texture::~Texture()
{
	
}

} // video
} // engine


