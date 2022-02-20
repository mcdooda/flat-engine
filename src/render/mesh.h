#ifndef FLAT_RENDER_MESH_H
#define FLAT_RENDER_MESH_H

#include <memory>
#include <vector>

#include "misc/vector.h"
#include "misc/matrix4.h"
#include "video/color.h"

namespace flat::video
{
	class Texture;
}

namespace flat::render
{

class Mesh
{
	friend class MeshBatch;

	using VertexPositions = std::vector<Vector3>;
	using VertexUvs = std::vector<Vector2>;

	public:
		Mesh();

		void setTexture(const std::shared_ptr<const video::Texture>& texture) { m_texture = texture; }
		inline const std::shared_ptr<const video::Texture>& getTexture() const { return m_texture; }

		inline void setColor(const video::Color& color) { m_color = color; }
		inline const video::Color& getColor() const { return m_color; }

		inline void setUseColor(bool useColor) { m_useColor = useColor; }
		inline bool getUseColor() const { return m_useColor; }

		inline void setRotation(const Vector3& rotation) { m_rotation = rotation; m_modelMatrixIsDirty = true; }
		inline void setRotationX(float rotationX) { m_rotation.x = rotationX; m_modelMatrixIsDirty = true; }
		inline void setRotationY(float rotationY) { m_rotation.y = rotationY; m_modelMatrixIsDirty = true; }
		inline void setRotationZ(float rotationZ) { m_rotation.z = rotationZ; m_modelMatrixIsDirty = true; }
		inline const Vector3& getRotation() const { return m_rotation; }

		inline void setScale(const Vector3& scale) { m_scale = scale; m_modelMatrixIsDirty = true; }
		inline void setScaleX(float scaleX) { m_scale.x = scaleX; m_modelMatrixIsDirty = true; }
		inline void setScaleY(float scaleY) { m_scale.y = scaleY; m_modelMatrixIsDirty = true; }
		inline void setScaleZ(float scaleZ) { m_scale.z = scaleZ; m_modelMatrixIsDirty = true; }
		inline const Vector3& getScale() const { return m_scale; }

		inline void setPosition(const Vector3& position) { m_position = position; m_modelMatrixIsDirty = true; }
		inline void setPositionX(float x) { m_position.x = x; m_modelMatrixIsDirty = true; }
		inline void setPositionY(float y) { m_position.y = y; m_modelMatrixIsDirty = true; }
		inline void setPositionZ(float z) { m_position.z = z; m_modelMatrixIsDirty = true; }
		inline const Vector3& getPosition() const { return m_position; }

		void clearVertices();
		void addVertex(const Vector3 position, Vector2 uv);

		void transform(const Matrix4& transform);

		void updateModelMatrix() const;
		inline const Matrix4& getModelMatrix() const { updateModelMatrix(); return m_modelMatrix; }

	protected:
		const VertexPositions& getVertexPositions() const;
		VertexPositions& getVertexPositions();
		const VertexUvs& getVertexUvs() const;
		VertexUvs& getVertexUvs();

	protected:
		std::shared_ptr<const video::Texture> m_texture;

		video::Color m_color;

		Vector3 m_rotation;
		Vector3 m_position;
		Vector3 m_scale;

		VertexPositions m_vertexPositions;
		VertexUvs m_vertexUvs;

		mutable Matrix4 m_modelMatrix;
		mutable bool m_modelMatrixIsDirty : 1;

		bool m_useColor : 1;
};

} // flat::render

#endif // FLAT_RENDER_BASESPRITE_H
