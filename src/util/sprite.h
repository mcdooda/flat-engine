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
		
		virtual Sprite* lightCopy();
		
		inline void setTexture(video::Texture* texture) { m_texture = texture; }
		inline video::Texture* getTexture() const { return m_texture; }
		
		inline void setColor(const video::Color& color) { m_color = color; }
		inline const video::Color& getColor() const { return m_color; }
		
		inline void setRotation(const geometry::Vector3& rotation) { m_rotation = rotation; m_modelMatrixIsDirty = true; }
		inline void setRotationX(float rotationX) { m_rotation.x = rotationX; m_modelMatrixIsDirty = true; }
		inline void setRotationY(float rotationY) { m_rotation.y = rotationY; m_modelMatrixIsDirty = true; }
		inline void setRotationZ(float rotationZ) { m_rotation.z = rotationZ; m_modelMatrixIsDirty = true; }
		inline const geometry::Vector3& getRotation() const { return m_rotation; }
		
		inline void setScale(float scale) { m_scale = scale; m_modelMatrixIsDirty = true; }
		inline float getScale() const { return m_scale; }
		
		inline void setPosition(const geometry::Vector2& position) { m_position = position; m_modelMatrixIsDirty = true; }
		inline void setPositionX(float x) { m_position.x = x; m_modelMatrixIsDirty = true; }
		inline void setPositionY(float y) { m_position.y = y; m_modelMatrixIsDirty = true; }
		inline const geometry::Vector2& getPosition() const { return m_position; }
		
		inline void moveBy(const geometry::Vector2& move) { m_position += move; m_modelMatrixIsDirty = true; }
		
		virtual void draw(const RenderSettings& renderSettings, const geometry::Matrix4& viewMatrix);
		
		void updateModelMatrix();
		inline const geometry::Matrix4& getModelMatrix() { updateModelMatrix(); return m_modelMatrix; }
		
	protected:
		video::Texture* m_texture;
		video::Color m_color;
		
		geometry::Vector3 m_rotation;
		geometry::Vector2 m_position;
		float m_scale;
		
		geometry::Matrix4 m_modelMatrix;
		bool m_modelMatrixIsDirty;
		
		bool m_isLightCopy;
};

} // util
} // flat

#endif // FLAT_UTIL_SPRITE_H



