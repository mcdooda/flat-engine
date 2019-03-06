#ifndef FLAT_RENDER_ANIMATEDSPRITE_H
#define FLAT_RENDER_ANIMATEDSPRITE_H

#include <iostream>

#include "render/sprite.h"
#include "render/spriteanimator.h"

namespace flat
{
namespace render
{

class AnimatedSprite : public Sprite, public SpriteAnimator
{
	public:
		AnimatedSprite();
		~AnimatedSprite() override;

	protected:
		const VertexUvs& getVertexUvs() const override;
		VertexUvs& getVertexUvs() override;

	private:
		VertexUvs m_vertexUvs;
};

} // render
} // flat

#endif // FLAT_RENDER_ANIMATEDSPRITE_H