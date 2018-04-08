#ifndef FLAT_RENDER_SPRITESYNCHRONIZER_H
#define FLAT_RENDER_SPRITESYNCHRONIZER_H

#include "basesprite.h"
#include "spriteanimator.h"

namespace flat
{
namespace render
{
class SynchronizedSprite;

class SpriteSynchronizer : public BaseSprite, public SpriteAnimator
{
	friend class SynchronizedSprite;
	public:
		SpriteSynchronizer();
		virtual ~SpriteSynchronizer();

	protected:
		const VertexPositions& getVertexPositions() const override;
		VertexPositions& getVertexPositions() override;
		const VertexUvs& getVertexUvs() const override;
		VertexUvs& getVertexUvs() override;

	private:
		VertexPositions m_vertexPositions;
		VertexUvs m_vertexUvs;
};

} // render
} // flat

#endif // FLAT_RENDER_SPRITESYNCHRONIZER_H


