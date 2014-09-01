#ifndef FLAT_GEOMETRY_RECTANGLE_H
#define FLAT_GEOMETRY_RECTANGLE_H

#include "polygon.h"

namespace flat
{
namespace geometry
{

class Rectangle : public Polygon
{
	public:
		Rectangle(const Vector2& position, const Vector2& size);
		Rectangle();
		virtual ~Rectangle();
		
		void setPositionSize(const Vector2& position, const Vector2& size);
		
		void setSize(const Vector2& size);
		Vector2 getSize() const;
		
		void setPosition(const Vector2& position);
		Vector2 getPosition() const;
		
		void draw(video::Attribute vertexAttribute, video::Attribute uvAttribute = 0) const;
		
};

} // geometry
} // flat

#endif // FLAT_GEOMETRY_RECTANGLE_H


