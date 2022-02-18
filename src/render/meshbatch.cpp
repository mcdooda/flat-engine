#include "meshbatch.h"
#include "mesh.h"
#include "rendersettings.h"

namespace flat::render
{

MeshBatch::MeshBatch() :
	m_numVertices(0),
	m_texture(nullptr)
{

}

void MeshBatch::clear()
{
	m_numVertices = 0;
	m_texture = nullptr;
}

void MeshBatch::add(const Mesh& mesh)
{
	if (m_numVertices == 0)
	{
		FLAT_ASSERT(m_texture == nullptr);
		m_texture = mesh.getTexture().get();
	}
#ifdef FLAT_DEBUG
	else
	{
		FLAT_ASSERT(m_numVertices > 0 && static_cast<size_t>(m_numVertices) + mesh.getVertexPositions().size() < m_vertices.size());
		FLAT_ASSERT(m_texture == mesh.getTexture().get());
	}
#endif
	FLAT_ASSERT(m_texture != nullptr);

	const Matrix4& transform = mesh.getModelMatrix();
	const video::Color& color = mesh.getColor();
	const float useColor = mesh.getUseColor() ? 1.f : 0.f;
	const Mesh::VertexPositions& vertexPositions = mesh.getVertexPositions();
	const Mesh::VertexUvs& vertexUvs = mesh.getVertexUvs();
	Vector4 localSpacePosition(0.f, 0.f, 0.f, 1.f);
	Vector4 worldSpacePosition(0.f, 0.f, 0.f, 1.f);
	for (int i = 0; i < mesh.getVertexPositions().size(); ++i)
	{
		FLAT_ASSERT(m_numVertices + 1 > 0);
		MeshBatch::Vertex& meshBatchVertex = m_vertices[m_numVertices++];
		FLAT_ASSERT(m_numVertices > 0);
		localSpacePosition.x = vertexPositions[i].x;
		localSpacePosition.y = vertexPositions[i].y;
		localSpacePosition.z = vertexPositions[i].z;
		worldSpacePosition = transform * localSpacePosition;
		meshBatchVertex.pos.x = worldSpacePosition.x;
		meshBatchVertex.pos.y = worldSpacePosition.y;
		meshBatchVertex.pos.z = worldSpacePosition.z;
		meshBatchVertex.uv = vertexUvs[i];
		meshBatchVertex.color = color;
		meshBatchVertex.useColor = useColor;
	}
}

void MeshBatch::draw(const RenderSettings& renderSettings, const Matrix4& viewMatrix) const
{
	FLAT_ASSERT(m_texture != nullptr);
	renderSettings.textureUniform.set(m_texture);

	const video::Attribute positionAttribute = renderSettings.positionAttribute;
	const video::Attribute uvAttribute = renderSettings.uvAttribute;
	const video::Attribute colorAttribute = renderSettings.colorAttribute;
	const video::Attribute useColorAttribute = renderSettings.useColorAttribute;

	glEnableVertexAttribArray(positionAttribute);
	glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &m_vertices[0].pos);

	glEnableVertexAttribArray(uvAttribute);
	glVertexAttribPointer(uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &m_vertices[0].uv);

	glEnableVertexAttribArray(colorAttribute);
	glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), &m_vertices[0].color);

	glEnableVertexAttribArray(useColorAttribute);
	glVertexAttribPointer(useColorAttribute, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), &m_vertices[0].useColor);

	glDrawArrays(GL_TRIANGLES, 0, m_numVertices);

	glDisableVertexAttribArray(positionAttribute);
	glDisableVertexAttribArray(uvAttribute);
	glDisableVertexAttribArray(colorAttribute);
	glDisableVertexAttribArray(useColorAttribute);
}

} // flat::render
