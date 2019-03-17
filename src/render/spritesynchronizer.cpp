#include "render/spritesynchronizer.h"

namespace flat
{
namespace render
{

SpriteSynchronizer::SpriteSynchronizer() : BaseSprite(), SpriteAnimator(this)
{
}

SpriteSynchronizer::~SpriteSynchronizer()
{
}

const BaseSprite::VertexPositions& SpriteSynchronizer::getVertexPositions() const
{
	FLAT_ASSERT(false);
	return m_vertexPositions;
}

BaseSprite::VertexPositions& SpriteSynchronizer::getVertexPositions()
{
	return m_vertexPositions;
}

const BaseSprite::VertexUvs& SpriteSynchronizer::getVertexUvs() const
{
	FLAT_ASSERT(false);
	return m_vertexUvs;
}

BaseSprite::VertexUvs& SpriteSynchronizer::getVertexUvs()
{
	return m_vertexUvs;
}

} // render
} // flat


