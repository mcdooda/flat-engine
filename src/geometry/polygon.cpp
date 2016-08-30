#include <cstring>
#include <GL/glew.h>
#include "polygon.h"

namespace flat
{
namespace geometry
{

Polygon::Polygon()
{
	
}

Polygon::Polygon(const std::vector<Vector2>& vertices) :
	m_vertices(vertices)
{
	
}

Polygon::Polygon(const Polygon& polygon)
{
	m_vertices = polygon.m_vertices;
}

void Polygon::operator=(const Polygon& polygon)
{
	m_vertices = polygon.m_vertices;
}

Polygon::~Polygon()
{
	
}

void Polygon::draw(video::Attribute vertexAttribute) const
{
	if (!m_vertices.empty())
	{
		glEnableVertexAttribArray(vertexAttribute);
		glVertexAttribPointer(vertexAttribute, 2, GL_FLOAT, GL_FALSE, 0, &m_vertices[0]);
		glDrawArrays(GL_POLYGON, 0, static_cast<GLsizei>(m_vertices.size()));
		glDisableVertexAttribArray(vertexAttribute);
	}
}

void Polygon::transform(const Matrix4& matrix4)
{
	for (std::vector<Vector2>::iterator it = m_vertices.begin(); it != m_vertices.end(); it++)
		*it = matrix4 * *it;
}

} // geometry
} // flat


