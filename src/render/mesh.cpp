#include "mesh.h"

namespace flat::render
{

Mesh::Mesh() :
	m_rotation(0.f),
	m_position(0.f),
	m_scale(1.f),
	m_modelMatrixIsDirty(true)
{

}

void Mesh::clearVertices()
{
	m_vertexPositions.clear();
	m_vertexUvs.clear();
}

void Mesh::addVertex(const Vector3 position, Vector2 uv)
{
	m_vertexPositions.push_back(position);
	m_vertexUvs.push_back(uv);
}

void Mesh::transform(const Matrix4& transform)
{
	for (Vector3& vertexPosition : m_vertexPositions)
	{
		vertexPosition = flat::Vector3(transform * flat::Vector4(vertexPosition, 1.f));
	}
}

void Mesh::updateModelMatrix() const
{
	if (m_modelMatrixIsDirty)
	{
		m_modelMatrixIsDirty = false;
		m_modelMatrix = Matrix4();
		translateBy(m_modelMatrix, m_position);
		rotateBy(m_modelMatrix, m_rotation);
		scaleBy(m_modelMatrix, m_scale);
	}
}

const Mesh::VertexPositions& Mesh::getVertexPositions() const
{
	return m_vertexPositions;
}

Mesh::VertexPositions& Mesh::getVertexPositions()
{
	return m_vertexPositions;
}

const Mesh::VertexUvs& Mesh::getVertexUvs() const
{
	return m_vertexUvs;
}

Mesh::VertexUvs& Mesh::getVertexUvs()
{
	return m_vertexUvs;
}

flat::Vector3& Mesh::getVertexPosition(size_t vertexIndex)
{
	return getVertexPositions().at(vertexIndex);
}

} // flat::render
