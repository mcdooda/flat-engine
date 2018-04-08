#include "synchronizedsprite.h"
#include "spritesynchronizer.h"

namespace flat
{
namespace render
{

SynchronizedSprite::SynchronizedSprite() : BaseSprite(),
	m_synchronizer(nullptr)
{

}

SynchronizedSprite::~SynchronizedSprite()
{
}

void SynchronizedSprite::synchronizeTo(SpriteSynchronizer& synchronizer)
{
	m_synchronizer = &synchronizer;
	setTexture(synchronizer.getTexture());
}

bool SynchronizedSprite::isSynchronized() const
{
	return m_synchronizer != nullptr;
}

SpriteSynchronizer& SynchronizedSprite::getSynchronizer() const
{
	FLAT_ASSERT(isSynchronized());
	return *m_synchronizer;
}

const BaseSprite::VertexPositions& SynchronizedSprite::getVertexPositions() const
{
	FLAT_ASSERT_MSG(m_synchronizer != nullptr, "Synchronized Sprite is not synchronized to a Sprite Synchronizer");
	return m_synchronizer->m_vertexPositions;
}

BaseSprite::VertexPositions& SynchronizedSprite::getVertexPositions()
{
	FLAT_ASSERT_MSG(m_synchronizer != nullptr, "Synchronized Sprite is not synchronized to a Sprite Synchronizer");
	return m_synchronizer->m_vertexPositions;
}

const BaseSprite::VertexUvs& SynchronizedSprite::getVertexUvs() const
{
	FLAT_ASSERT_MSG(m_synchronizer != nullptr, "Synchronized Sprite is not synchronized to a Sprite Synchronizer");
	return m_synchronizer->m_vertexUvs;
}

BaseSprite::VertexUvs& SynchronizedSprite::getVertexUvs()
{
	FLAT_ASSERT_MSG(m_synchronizer != nullptr, "Synchronized Sprite is not synchronized to a Sprite Synchronizer");
	return m_synchronizer->m_vertexUvs;
}

} // render
} // flat

