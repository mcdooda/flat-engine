#include <cmath>
#include <cstdlib>
#include "circle.h"

namespace flat
{
namespace geometry
{

Circle::Circle(const Vector2& center, float radius) : Polygon(),
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
	const int numVertices = 64;
	m_vertices.clear();
	m_vertices.reserve(numVertices);
	for (int i = 0; i < numVertices; i++)
	{
		float alpha = M_PI * 2 * static_cast<float>(i) / numVertices;
		m_vertices.emplace_back(m_center.x + cos(alpha) * m_radius, m_center.y + sin(alpha) * m_radius);
	}
}

} // geometry
} // flat


