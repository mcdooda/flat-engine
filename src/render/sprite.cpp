#include "render/sprite.h"

#include "video/filetexture.h"
#include "misc/aabb2.h"

namespace flat
{
namespace render
{

BaseSprite::VertexUvs Sprite::vertexUvs = {
	Vector2(0.f, 0.f),
	Vector2(1.f, 0.f),
	Vector2(0.f, 1.f),
	Vector2(1.f, 1.f),
	Vector2(0.f, 1.f),
	Vector2(1.f, 0.f)
};

Sprite::Sprite() : BaseSprite()
{
	// pos
	m_vertexPositions[0].x = 0.f;
	m_vertexPositions[0].y = 0.f;

	m_vertexPositions[1].x = 1.f; // will change according to the texture size
	m_vertexPositions[1].y = 0.f;

	m_vertexPositions[2].x = 0.f;
	m_vertexPositions[2].y = 1.f; // will change according to the texture size

	m_vertexPositions[3].x = 1.f; // will change according to the texture size
	m_vertexPositions[3].y = 1.f; // will change according to the texture size

	m_vertexPositions[4].x = 0.f;
	m_vertexPositions[4].y = 1.f; // will change according to the texture size

	m_vertexPositions[5].x = 1.f; // will change according to the texture size
	m_vertexPositions[5].y = 0.f;
}

Sprite::~Sprite()
{

}

void Sprite::setTexture(const std::shared_ptr<const video::Texture>& texture)
{
	BaseSprite::setTexture(texture);

	const Vector2& textureSize = m_texture->getSize();
	m_vertexPositions[1].x = textureSize.x;
	m_vertexPositions[2].y = textureSize.y;
	m_vertexPositions[3].x = textureSize.x;
	m_vertexPositions[3].y = textureSize.y;
	m_vertexPositions[4].y = textureSize.y;
	m_vertexPositions[5].x = textureSize.x;
}

const BaseSprite::VertexPositions& Sprite::getVertexPositions() const
{
	return m_vertexPositions;
}

BaseSprite::VertexPositions& Sprite::getVertexPositions()
{
	return m_vertexPositions;
}

const BaseSprite::VertexUvs& Sprite::getVertexUvs() const
{
	return vertexUvs;
}

BaseSprite::VertexUvs& Sprite::getVertexUvs()
{
	FLAT_ASSERT_MSG(false, "Only getVertexUvs() *const* can be called from Sprite");
	return vertexUvs;
}

} // render
} // flat



