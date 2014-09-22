#ifndef FLAT_GEOMETRY_VECTOR2_H
 #define FLAT_GEOMETRY_VECTOR2_H

#include <ostream>

#ifndef M_PI
	#define M_PI 3.1415926535897932384626433832795
#endif

namespace flat
{
namespace geometry
{

class Vector2
{
	public:
		Vector2(float x, float y);
		Vector2(float alpha);
		Vector2();
		Vector2(const Vector2& vector2);
		void operator=(const Vector2& vector2);

		float distance() const;
		float distanceSquared() const;
		float angle() const;
		Vector2 normalize() const;
		
		float dotProduct(const Vector2& v) const;

		Vector2 operator+(const Vector2& v) const;
		Vector2 operator-(const Vector2& v) const;
		Vector2 operator*(const float& f) const;
		Vector2 operator/(const float& f) const;

		void operator+=(const Vector2& v);
		void operator-=(const Vector2& v);
		void operator*=(const float& f);
		void operator/=(const float& f);

		inline void setX(float x) { m_x = x; }
		inline void setY(float y) { m_y = y; }

		inline float getX() const { return m_x; }
		inline float getY() const { return m_y; }

		int getRoundX() const;
		int getRoundY() const;
		
		friend std::ostream& operator<<(std::ostream& out, Vector2 vector2);

	protected:
		float m_x;
		float m_y;
};

} // geometry
} // flat

#endif // FLAT_GEOMETRY_VECTOR2_H


