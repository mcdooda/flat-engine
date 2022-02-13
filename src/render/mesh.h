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
