#ifndef FLAT_RENDER_SPRITE_H
#define FLAT_RENDER_SPRITE_H

#include "render/basesprite.h"

namespace flat
{
namespace render
{

class Sprite : public BaseSprite
{
	public:
		Sprite();
		virtual ~Sprite();

		void setTexture(const std::shared_ptr<const video::Texture>& texture); // not virtual!

	protected:
		const VertexPositions& getVertexPositions() const override;
		VertexPositions& getVertexPositions() override;
		const VertexUvs& getVertexUvs() const override;
		VertexUvs& getVertexUvs() override;

	private:
		VertexPositions m_vertexPositions;
		static VertexUvs vertexUvs;
};

} // render
} // flat

#endif // FLAT_RENDER_SPRITE_H


