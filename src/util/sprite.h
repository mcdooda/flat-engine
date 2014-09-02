#ifndef FLAT_UTIL_SPRITE_H
#define FLAT_UTIL_SPRITE_H

#include "rendersettings.h"
#include "../video/texture.h"

namespace flat
{
namespace util
{

class Sprite
{
	public:
		Sprite();
		virtual ~Sprite();
		
		inline void setTexture(video::Texture* texture) { m_texture = texture; }
		inline void setRotation(float rotation) { m_rotation = rotation; }
		inline void setScale(float scale) { m_scale = scale; }
		inline void setPosition(const geometry::Vector2& position) { m_position = position; }
		
		void draw(const RenderSettings& renderSettings) const;
		
	protected:
		video::Texture* m_texture;
		float m_rotation;
		float m_scale;
		geometry::Vector2 m_position;
		
};

} // util
} // flat

#endif // FLAT_UTIL_SPRITE_H



