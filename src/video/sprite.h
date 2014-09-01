#ifndef FLAT_VIDEO_SPRITE_H
#define FLAT_VIDEO_SPRITE_H

#include "texture.h"
#include "uniform.h"
#include "../geometry/rectangle.h"

namespace flat
{
namespace video
{

class Sprite
{
	public:
		Sprite(Texture* texture);
		virtual ~Sprite();
		
		void setPosition(const geometry::Vector2& position);
		
		void draw(const Uniform& textureUniform, const Attribute& positionAttribute, const Attribute& levelUvAttribute) const;
		
	private:
		Texture* m_texture;
		geometry::Rectangle m_rectangle;
		
};

} // video
} // flat

#endif // FLAT_VIDEO_SPRITE_H



