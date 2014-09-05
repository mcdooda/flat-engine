#ifndef FLAT_UTIL_SPRITE_H
#define FLAT_UTIL_SPRITE_H

#include "rendersettings.h"
#include "../video/texture.h"
#include "../geometry/vector3.h"

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
		inline video::Texture* getTexture() const { return m_texture; }
		
		inline void setRotation(const geometry::Vector3& rotation) { m_rotation = rotation; }
		inline void setRotationX(float rotationX) { m_rotation.setX(rotationX); }
		inline void setRotationY(float rotationY) { m_rotation.setY(rotationY); }
		inline void setRotationZ(float rotationZ) { m_rotation.setZ(rotationZ); }
		inline const geometry::Vector3& getRotation() const { return m_rotation; }
		
		inline void setScale(float scale) { m_scale = scale; }
		inline float getScale() const { return m_scale; }
		
		inline void setPosition(const geometry::Vector2& position) { m_position = position; }
		inline const geometry::Vector2& getPosition() const { return m_position; }
		
		inline void moveBy(const geometry::Vector2& move) { m_position += move; }
		
		virtual void draw(const RenderSettings& renderSettings) const;
		
		virtual geometry::Matrix4 getModelMatrix() const;
		
	protected:
		video::Texture* m_texture;
		geometry::Vector3 m_rotation;
		float m_scale;
		geometry::Vector2 m_position;
		
};

} // util
} // flat

#endif // FLAT_UTIL_SPRITE_H


