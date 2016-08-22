#ifndef FLAT_GEOMETRY_VECTOR3_H
#define FLAT_GEOMETRY_VECTOR3_H

#include <cmath>
#include <ostream>
#include "../debug/assert.h"

namespace flat
{

class Vector3
{
	public:
		inline Vector3() :
			x(0.f),
			y(0.f),
			z(0.f)
		{

		}

		inline Vector3(const Vector3& vector3) :
			x(vector3.x),
			y(vector3.y),
			z(vector3.z)
		{
	
		}

		inline Vector3(float x, float y, float z) :
			x(x),
			y(y),
			z(z)
		{

		}

		inline void operator=(const Vector3& vector3)
		{
			x = vector3.x;
			y = vector3.y;
			z = vector3.z;
		}

		inline float length() const
		{
			return sqrt(x * x + y * y + z * z);
		}

		inline float lengthSquared() const
		{
			return x * x + y * y + z * z;
		}

		inline Vector3 normalize() const
		{
			float len = length();

			if (len != 0)
				return Vector3(x / len, y / len, z / len);

			else
				return *this;
		}

		inline float dotProduct(const Vector3& v) const
		{
			return x * v.x + y * v.y + z * v.z;
		}

		inline Vector3 crossProduct(const Vector3& v) const
		{
			return Vector3(
				y * v.z - z * v.y,
				z * v.x - x * v.z,
				x * v.y - y * v.x
			);
		}

		inline Vector3 operator+(const Vector3& v) const
		{
			return Vector3(x + v.x, y + v.y, z + v.z);
		}

		inline Vector3 operator-(const Vector3& v) const
		{
			return Vector3(x - v.x, y - v.y, z - v.z);
		}

		inline Vector3 operator*(float f) const
		{
			return Vector3(x * f, y * f, z * f);
		}

		inline Vector3 operator/(float f) const
		{
			FLAT_ASSERT(f != 0.f);
			return Vector3(x / f, y / f, z / f);
		}

		inline void operator+=(const Vector3& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
		}

		inline void operator-=(const Vector3& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
		}

		inline void operator*=(float f)
		{
			x *= f;
			y *= f;
			z *= f;
		}

		inline void operator/=(float f)
		{
			FLAT_ASSERT(f != 0.f);
			x /= f;
			y /= f;
			z /= f;
		}

		inline Vector3 operator-() const
		{
			return Vector3(-x, -y, -z);
		}

		inline int getRoundX() const
		{
			return static_cast<int>(std::round(x));
		}

		inline int getRoundY() const
		{
			return static_cast<int>(std::round(y));
		}

		inline int getRoundZ() const
		{
			return static_cast<int>(std::round(z));
		}
		
		friend std::ostream& operator<<(std::ostream& out, const Vector3& vector3);

	public:
		float x;
		float y;
		float z;
};

static_assert(sizeof(float) * 3 == sizeof(Vector3), "no padding");

inline std::ostream& operator<<(std::ostream& out, const Vector3& vector3)
{
	out << "Vector3(" << vector3.x << "," << vector3.y << "," << vector3.z << ")";
	return out;
}

} // flat

#endif // FLAT_GEOMETRY_VECTOR3_H



