#include <cmath>
#include <cstdlib>
#include "circle.h"

namespace flat
{
namespace geometry
{

Circle::Circle(Vector2 center, float radius) : Polygon(),
	m_center(center),
	m_radius(radius)
{
	buildCircle();
}

Circle::Circle() : Polygon()
{
	
}

Circle::~Circle()
{
	
}

void Circle::setCenter(const Vector2& center)
{
	m_center = center;
	buildCircle();
}

void Circle::setRadius(float radius)
{
	m_radius = radius;
	buildCircle();
}

void Circle::buildCircle()
{
	const int numVertices = 512;
	m_vertices.clear();
	m_vertices.reserve(numVertices);
	for (int i = 0; i < numVertices; i++)
	{
		m_vertices.push_back(Vector2(
			m_center.getX() + cos(M_PI * 2 * (float) i / numVertices) * m_radius,
			m_center.getY() + sin(M_PI * 2 * (float) i / numVertices) * m_radius
		));
	}
}

} // geometry
} // flat


