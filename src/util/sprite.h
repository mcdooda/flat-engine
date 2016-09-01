#ifndef FLAT_UTIL_SPRITE_H
#define FLAT_UTIL_SPRITE_H

#include <memory>
#include "rendersettings.h"
#include "../video/texture.h"
#include "../misc/vector3.h"
#include "../containers/fixedsizearray.h"

namespace flat
{
namespace util
{
class SpriteBatch;

class Sprite
{
	friend class SpriteBatch;

	public:
		Sprite();
		virtual ~Sprite();
		
		virtual Sprite* lightCopy();
		
		void setTexture(const std::shared_ptr<const video::Texture>& texture);
		inline const std::shared_ptr<const video::Texture>& getTexture() const { return m_texture; }
		
		inline void setColor(const video::Color& color) { m_color = color; }
		inline const video::Color& getColor() const { return m_color; }
		
		inline void setRotation(const Vector3& rotation) { m_rotation = rotation; m_modelMatrixIsDirty = true; }
		inline void setRotationX(float rotationX) { m_rotation.x = rotationX; m_modelMatrixIsDirty = true; }
		inline void setRotationY(float rotationY) { m_rotation.y = rotationY; m_modelMatrixIsDirty = true; }
		inline void setRotationZ(float rotationZ) { m_rotation.z = rotationZ; m_modelMatrixIsDirty = true; }
		inline const Vector3& getRotation() const { return m_rotation; }
		
		inline void setScale(float scale) { m_scale = scale; m_modelMatrixIsDirty = true; }
		inline float getScale() const { return m_scale; }
		
		inline void setPosition(const Vector2& position) { m_position = position; m_modelMatrixIsDirty = true; }
		inline void setPositionX(float x) { m_position.x = x; m_modelMatrixIsDirty = true; }
		inline void setPositionY(float y) { m_position.y = y; m_modelMatrixIsDirty = true; }
		inline const Vector2& getPosition() const { return m_position; }
		
		inline void setOrigin(const Vector2& origin) { m_origin = origin; m_modelMatrixIsDirty = true; }
		inline void setOriginX(float x) { m_origin.x = x; m_modelMatrixIsDirty = true; }
		inline void setOriginY(float y) { m_origin.y = y; m_modelMatrixIsDirty = true; }
		inline const Vector2& getOrigin() const { return m_origin; }
		
		inline void setFlipX(bool flipX) { m_flipX = flipX; m_modelMatrixIsDirty = true; }
		inline bool getFlipX() const { return m_flipX; }
		inline void setFlipY(bool flipY) { m_flipY = flipY; m_modelMatrixIsDirty = true; }
		inline bool getFlipY() const { return m_flipY; }
		
		inline void moveBy(const Vector2& move) { m_position += move; m_modelMatrixIsDirty = true; }
		
		virtual void draw(const RenderSettings& renderSettings, const Matrix4& viewMatrix) const;
		
		void updateModelMatrix() const;
		inline const Matrix4& getModelMatrix() const { updateModelMatrix(); return m_modelMatrix; }
		
		struct Vertex
		{
			flat::Vector2 pos;
			flat::Vector2 uv;
		};
		
	protected:
		std::shared_ptr<const video::Texture> m_texture;
		
		flat::containers::FixedSizeArray<Vertex, 6> m_vertices;
		
		video::Color m_color;
		
		Vector3 m_rotation;
		Vector2 m_position;
		Vector2 m_origin;
		float m_scale;
		
		bool m_flipX : 1;
		bool m_flipY : 1;
		
		bool m_isLightCopy : 1;
		
		mutable bool m_modelMatrixIsDirty : 1;
		mutable Matrix4 m_modelMatrix;
};

} // util
} // flat

#endif // FLAT_UTIL_SPRITE_H



