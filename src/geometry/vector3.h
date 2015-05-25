#ifndef FLAT_GEOMETRY_VECTOR3_H
#define FLAT_GEOMETRY_VECTOR3_H

#include <ostream>

namespace flat
{
namespace geometry
{

class Vector3
{
	public:
		Vector3();
		Vector3(const Vector3& vector3);
		Vector3(float x, float y, float z);
		void operator=(const Vector3& vector3);

		float length() const;
		float lengthSquared() const;
		Vector3 normalize() const;
		
		float dotProduct(const Vector3& v) const;
		Vector3 crossProduct(const Vector3& v) const;

		Vector3 operator+(const Vector3& v) const;
		Vector3 operator-(const Vector3& v) const;
		Vector3 operator*(const float& f) const;
		Vector3 operator/(const float& f) const;

		void operator+=(const Vector3& v);
		void operator-=(const Vector3& v);
		void operator*=(const float& f);
		void operator/=(const float& f);

		int getRoundX() const;
		int getRoundY() const;
		int getRoundZ() const;
		
		friend std::ostream& operator<<(std::ostream& out, Vector3 vector3);

	public:
		float x;
		float y;
		float z;
};

static_assert(sizeof(float) * 3 == sizeof(Vector3), "no padding");

} // geometry
} // flat

#endif // FLAT_GEOMETRY_VECTOR3_H



