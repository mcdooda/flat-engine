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
		Vector2();
		Vector2(const Vector2& vector2);
		Vector2(float x, float y);
		explicit Vector2(float alpha);
		void operator=(const Vector2& vector2);

		float length() const;
		float lengthSquared() const;
		float angle() const;
		Vector2 normalize() const;
		
		float dotProduct(const Vector2& v) const;

		Vector2 operator+(const Vector2& v) const;
		Vector2 operator-(const Vector2& v) const;
		Vector2 operator*(float f) const;
		Vector2 operator/(float f) const;

		void operator+=(const Vector2& v);
		void operator-=(const Vector2& v);
		void operator*=(float f);
		void operator/=(float f);

		Vector2 operator-() const;

		int getRoundX() const;
		int getRoundY() const;
		
		friend std::ostream& operator<<(std::ostream& out, const Vector2& vector2);

	public:
		float x;
		float y;
};

static_assert(sizeof(float) * 2 == sizeof(Vector2), "no padding");

} // geometry
} // flat

#endif // FLAT_GEOMETRY_VECTOR2_H


