#include <cmath>

#include "render/animatedsprite.h"

#include "misc/aabb2.h"
#include "video/filetexture.h"

namespace flat
{
namespace render
{

AnimatedSprite::AnimatedSprite() : Sprite(), SpriteAnimator(this)
{
	
}

AnimatedSprite::~AnimatedSprite()
{
	
}

const BaseSprite::VertexUvs& AnimatedSprite::getVertexUvs() const
{
	return m_vertexUvs;
}

BaseSprite::VertexUvs& AnimatedSprite::getVertexUvs()
{
	return m_vertexUvs;
}

} // render
} // flat



