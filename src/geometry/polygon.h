#ifndef FLAT_VIDEO_POLYGON_H
#define FLAT_VIDEO_POLYGON_H

#include <vector>

#include "misc/vector.h"
#include "misc/matrix4.h"
#include "video/attribute.h"

namespace flat
{
namespace geometry
{

class Polygon
{
	public:
		Polygon();
		Polygon(const std::vector<Vector2>& vertices);
		Polygon(const Polygon& polygon);
		void operator=(const Polygon& polygon);
		virtual ~Polygon();
		
		inline void setVertices(const std::vector<Vector2>& vertices) { m_vertices = vertices; }
		
		void draw(video::Attribute vertexAttribute) const;
		
		void transform(const Matrix4& matrix4);
		
	protected:
		std::vector<Vector2> m_vertices;
};

} // geometry
} // flat

#endif // FLAT_VIDEO_POLYGON_H


