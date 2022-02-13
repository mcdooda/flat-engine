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

} // flat::render
