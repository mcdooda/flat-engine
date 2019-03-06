#include <cstring>
#include <iostream>

#include "geometry/rectangle.h"

namespace flat
{
namespace geometry
{

Rectangle::Rectangle(const Vector2& position, const Vector2& size)
{
	setPositionSize(position, size);
}

Rectangle::Rectangle()
{
	setPositionSize(Vector2(), Vector2());
}

Rectangle::~Rectangle()
{
	
}

void Rectangle::setPositionSize(const Vector2& position, const Vector2& size)
{
	m_vertices.clear();
	m_vertices.reserve(4);
	m_vertices.push_back(position);
	m_vertices.emplace_back(position.x + size.x, position.y);
	m_vertices.push_back(position + size);
	m_vertices.emplace_back(position.x, position.y + size.y);
}

void Rectangle::setSize(const Vector2& size)
{
	Vector2 position = getPosition();
	setPositionSize(position, size);
}

Vector2 Rectangle::getSize() const
{
	float width = m_vertices[1].x - m_vertices[0].x;
	float height = m_vertices[3].y - m_vertices[0].y;
	return Vector2(width, height);
}

void Rectangle::setPosition(const Vector2& position)
{
	Vector2 size = getSize();
	setPositionSize(position, size);
}

const Vector2& Rectangle::getPosition() const
{
	return m_vertices[0];
}

void Rectangle::draw(video::Attribute vertexAttribute, video::Attribute uvAttribute) const
{
	if (!m_vertices.empty())
	{
		glEnableVertexAttribArray(vertexAttribute);
		glVertexAttribPointer(vertexAttribute, 2, GL_FLOAT, GL_FALSE, 0, &m_vertices[0]);
		if (uvAttribute != 0)
		{
			static const float uv[] = {
				0.0f,0.0f,
				1.0f,0.0f,
				1.0f,1.0f,
				0.0f,1.0f
			};
			glEnableVertexAttribArray(uvAttribute);
			glVertexAttribPointer(uvAttribute, 2, GL_FLOAT, GL_FALSE, 0, uv);
		}
		glDrawArrays(GL_QUADS, 0, 4);
		glDisableVertexAttribArray(vertexAttribute);
		if (uvAttribute != 0)
			glDisableVertexAttribArray(uvAttribute);
	}
}

bool Rectangle::contains(const Vector2& v)
{
	const Vector2& position = getPosition();
	Vector2 size = getSize();
	return v.x >= position.x && v.x <= position.x + size.x
	    && v.y >= position.y && v.y <= position.y + size.y;
}

} // geometry
} // flat


