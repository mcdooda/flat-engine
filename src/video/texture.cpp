#include "texture.h"

namespace flat
{
namespace video
{

Texture::Texture() :
	m_textureId(0)
{
	
}

Texture::Texture(GLuint textureId, const Vector2& size, const std::string& name) :
	m_textureId(textureId),
	m_size(size),
	m_name(name)
{
	
}

Texture::~Texture()
{
	
}

} // video
} // flat


