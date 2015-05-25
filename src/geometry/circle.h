#ifndef FLAT_GEOMETRY_CIRCLE_H
#define FLAT_GEOMETRY_CIRCLE_H

#include "polygon.h"

namespace flat
{
namespace geometry
{

class Circle : public Polygon
{
	public:
		Circle(const Vector2& center, float radius);
		Circle();
		~Circle() override;
		
		void setCenter(const Vector2& center);
		void setRadius(float radius);
		
		inline const Vector2& getCenter() const { return m_center; }
		inline float getRadius() const { return m_radius; }
		
	private:
		void buildCircle();
		
	private:
		Vector2 m_center;
		float m_radius;
};

} // geometry
} // flat

#endif // FLAT_GEOMETRY_CIRCLE_H


