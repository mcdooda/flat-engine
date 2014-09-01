#include "sprite.h"

namespace flat
{
namespace video
{

Sprite::Sprite(Texture* texture) :
	m_texture(texture),
	m_rectangle(texture->getSize() / -2.f, texture->getSize())
{
	
}

Sprite::~Sprite()
{
	
}

void Sprite::setPosition(const geometry::Vector2& position)
{
	m_rectangle.setPosition(position - m_texture->getSize() / 2.f);
}

void Sprite::draw(const Uniform& textureUniform, const Attribute& positionAttribute, const Attribute& levelUvAttribute) const
{
	textureUniform.setTexture(m_texture);
	m_rectangle.draw(positionAttribute, levelUvAttribute);
}

} // video
} // flat



