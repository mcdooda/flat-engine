#ifndef FLAT_RENDER_BASESPRITE_H
#define FLAT_RENDER_BASESPRITE_H

#include <array>
#include <memory>

#include "misc/vector.h"
#include "misc/matrix4.h"
#include "video/color.h"

namespace flat
{
class AABB2;

namespace video
{
class Texture;
}

namespace render
{
class SpriteBatch;
class SpriteAnimator;
struct RenderSettings;

class BaseSprite
{
	friend class SpriteBatch;
	friend class SpriteAnimator;

	public:
		BaseSprite();
		virtual ~BaseSprite();
		
		void setTexture(const std::shared_ptr<const video::Texture>& texture);
		inline const std::shared_ptr<const video::Texture>& getTexture() const { return m_texture; }
		
		inline void setColor(const video::Color& color) { m_color = color; }
		inline const video::Color& getColor() const { return m_color; }

		inline void setNormal(const Vector3& normal) { m_normal = normal; }
		inline const Vector3& getNormal() const { return m_normal; }

		inline void setDepth(float depth) { m_depth = depth; }
		inline float getDepth() const { return m_depth; }
		
		inline void setRotation(const Vector3& rotation) { m_rotation = rotation; m_modelMatrixIsDirty = true; }
		inline void setRotationX(float rotationX) { m_rotation.x = rotationX; m_modelMatrixIsDirty = true; }
		inline void setRotationY(float rotationY) { m_rotation.y = rotationY; m_modelMatrixIsDirty = true; }
		inline void setRotationZ(float rotationZ) { m_rotation.z = rotationZ; m_modelMatrixIsDirty = true; }
		inline const Vector3& getRotation() const { return m_rotation; }
		
		inline void setScale(const Vector2& scale) { m_scale = scale; m_modelMatrixIsDirty = true; }
		inline void setScaleX(float scaleX) { m_scale.x = scaleX; m_modelMatrixIsDirty = true; }
		inline void setScaleY(float scaleY) { m_scale.y = scaleY; m_modelMatrixIsDirty = true; }
		inline const Vector2& getScale() const { return m_scale; }
		
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

		void setModelMatrix(const Matrix4& modelMatrix);

		void getAABB(AABB2& aabb) const;

		void getPixel(const Vector2& point, video::Color& color) const;

		bool requiresAlphaBlending() const;
		
	protected:
		static constexpr int NUM_VERTICES = 6;
		using VertexPositions = std::array<Vector2, NUM_VERTICES>;
		using VertexUvs = std::array<Vector2, NUM_VERTICES>;

		virtual const VertexPositions& getVertexPositions() const = 0;
		virtual VertexPositions& getVertexPositions() = 0;
		virtual const VertexUvs& getVertexUvs() const = 0;
		virtual VertexUvs& getVertexUvs() = 0;

	protected:
		std::shared_ptr<const video::Texture> m_texture;
		
		video::Color m_color;
		Vector3 m_normal;
		float m_depth;
		
		Vector3 m_rotation;
		Vector2 m_position;
		Vector2 m_origin;
		Vector2 m_scale;
		
		bool m_flipX : 1;
		bool m_flipY : 1;
		
		mutable bool m_modelMatrixIsDirty : 1;
		mutable Matrix4 m_modelMatrix;
};

}
}

#endif // FLAT_RENDER_BASESPRITE_H


