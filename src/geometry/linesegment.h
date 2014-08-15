#ifndef FLAT_GEOMETRY_LINESEGMENT_H
#define FLAT_GEOMETRY_LINESEGMENT_H

#include "vector2.h"
#include "../video/attribute.h"

namespace flat
{
namespace geometry
{

class LineSegment
{
	public:
		LineSegment(Vector2 a, Vector2 b);
		LineSegment();
		virtual ~LineSegment();
		
		void draw(video::Attribute vertexAttribute);
		
		inline Vector2 getA() const { return Vector2(m_vertices[0], m_vertices[1]); }
		void setA(const Vector2& a) { m_vertices[0] = a.getX(); m_vertices[1] = a.getY(); }
		
		inline Vector2 getB() const { return Vector2(m_vertices[2], m_vertices[3]); }
		void setB(const Vector2& b) { m_vertices[2] = b.getX(); m_vertices[3] = b.getY(); }
		
	private:
		float m_vertices[4];
};

} // geometry
} // flat

#endif // FLAT_GEOMETRY_LINESEGMENT_H


