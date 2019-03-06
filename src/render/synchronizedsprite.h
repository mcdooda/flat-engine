#ifndef FLAT_RENDER_SYNCHRONIZEDSPRITE_H
#define FLAT_RENDER_SYNCHRONIZEDSPRITE_H

#include "render/sprite.h"

namespace flat
{
namespace render
{
class SpriteSynchronizer;

class SynchronizedSprite : public BaseSprite
{
	public:
		SynchronizedSprite();
		virtual ~SynchronizedSprite();

		void synchronizeTo(SpriteSynchronizer& synchronizer);

		bool isSynchronized() const;

		SpriteSynchronizer& getSynchronizer() const;

	protected:
		const VertexPositions& getVertexPositions() const override;
		VertexPositions& getVertexPositions() override;
		const VertexUvs& getVertexUvs() const override;
		VertexUvs& getVertexUvs() override;

	private:
		SpriteSynchronizer* m_synchronizer;
};

} // render
} // flat

#endif // FLAT_RENDER_SYNCHRONIZEDSPRITE_H


