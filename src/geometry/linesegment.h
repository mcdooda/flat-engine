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
		LineSegment(const Vector2& a, const Vector2& b);
		LineSegment();
		~LineSegment();
		
		void draw(video::Attribute vertexAttribute);
		
		inline const Vector2& getA() const { return m_a; }
		void setA(const Vector2& a) { m_a = a; }
		
		inline const Vector2& getB() const { return m_b; }
		void setB(const Vector2& b) { m_b = b; }
		
	private:
		Vector2 m_a;
		Vector2 m_b;
};

static_assert(sizeof(LineSegment) == (sizeof(Vector2) * 2), "there should not be any padding between A and B!");

} // geometry
} // flat

#endif // FLAT_GEOMETRY_LINESEGMENT_H


