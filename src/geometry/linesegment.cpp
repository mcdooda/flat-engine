#include <GL/glew.h>
#include "linesegment.h"

namespace flat
{
namespace geometry
{

LineSegment::LineSegment(const Vector2& a, const Vector2& b) :
	m_a(a),
	m_b(b)
{
	
}

LineSegment::LineSegment()
{
	
}

LineSegment::~LineSegment()
{

}

void LineSegment::draw(video::Attribute vertexAttribute)
{
	glEnableVertexAttribArray(vertexAttribute);
	glVertexAttribPointer(vertexAttribute, 2, GL_FLOAT, GL_FALSE, 0, this);
	glDrawArrays(GL_LINES, 0, 2);
	glDisableVertexAttribArray(vertexAttribute);
}

} // geometry
} //flat


