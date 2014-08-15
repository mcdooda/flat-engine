#include <GL/glew.h>
#include "linesegment.h"

namespace engine
{
namespace geometry
{

LineSegment::LineSegment(Vector2 a, Vector2 b)
{
	setA(a);
	setB(b);
}

LineSegment::LineSegment()
{
	setA(Vector2());
	setB(Vector2());
}

LineSegment::~LineSegment()
{

}

void LineSegment::draw(video::Attribute vertexAttribute)
{
	glEnableVertexAttribArray(vertexAttribute);
	glVertexAttribPointer(vertexAttribute, 2, GL_FLOAT, GL_FALSE, 0, m_vertices);
	glDrawArrays(GL_LINES, 0, 2);
	glDisableVertexAttribArray(vertexAttribute);
}

} // geometry
} //engine


