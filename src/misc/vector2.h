#ifndef FLAT_GEOMETRY_VECTOR2_H
#define FLAT_GEOMETRY_VECTOR2_H

#include <cmath>
#include <ostream>
#include "../debug/assert.h"

#ifndef M_PI
	#define M_PI 3.1415926535897932384626433832795
#endif

namespace flat
{

class Vector2
{
	public:
		inline Vector2() :
			x(0.f),
			y(0.f)
		{

		}

		inline Vector2(const Vector2& vector2) :
			x(vector2.x),
			y(vector2.y)
		{
	
		}

		inline Vector2(float x, float y) :
			x(x),
			y(y)
		{

		}

		inline Vector2(float alpha)
		{
			x = cos(alpha);
			y = sin(alpha);
		}

		inline void operator=(const Vector2& vector2)
		{
			x = vector2.x;
			y = vector2.y;
		}

		inline float length() const
		{
			return sqrt(x * x + y * y);
		}

		inline float lengthSquared() const
		{
			return x * x + y * y;
		}

		inline float angle() const
		{
			FLAT_ASSERT(x != 0 || y != 0);
			return atan2f(y, x);
		}

		inline Vector2 normalize() const
		{
			float len = length();

			if (len != 0)
				return Vector2(x / len, y / len);

			else
				return *this;
		}

		inline float dotProduct(const Vector2& v) const
		{
			return x * v.x + y * v.y;
		}

		inline Vector2 operator+(const Vector2& v) const
		{
			return Vector2(x + v.x, y + v.y);
		}

		inline Vector2 operator-(const Vector2& v) const
		{
			return Vector2(x - v.x, y - v.y);
		}

		inline Vector2 operator*(float f) const
		{
			return Vector2(x * f, y * f);
		}

		inline Vector2 operator/(float f) const
		{
			FLAT_ASSERT(f != 0.f);
			return Vector2(x / f, y / f);
		}

		inline void operator+=(const Vector2& v)
		{
			x += v.x;
			y += v.y;
		}

		inline void operator-=(const Vector2& v)
		{
			x -= v.x;
			y -= v.y;
		}

		inline void operator*=(float f)
		{
			x *= f;
			y *= f;
		}

		inline void operator/=(float f)
		{
			FLAT_ASSERT(f != 0.f);
			x /= f;
			y /= f;
		}

		inline Vector2 operator-() const
		{
			return Vector2(-x, -y);
		}

		inline int getRoundX() const
		{
			return static_cast<int>(std::round(x));
		}

		inline int getRoundY() const
		{
			return static_cast<int>(std::round(y));
		}
		
		friend std::ostream& operator<<(std::ostream& out, const Vector2& vector2);

	public:
		float x;
		float y;
};

static_assert(sizeof(float) * 2 == sizeof(Vector2), "no padding");

inline std::ostream& operator<<(std::ostream& out, const Vector2& vector2)
{
	out << "Vector2(" << vector2.x << "," << vector2.y << ")";
	return out;
}

} // flat

#endif // FLAT_GEOMETRY_VECTOR2_H


