#ifndef FLAT_GEOMETRY_VECTOR3_H
#define FLAT_GEOMETRY_VECTOR3_H

#include "vector2.h"

namespace flat
{
namespace geometry
{

class Vector3 : public Vector2
{
	public:
		Vector3(float x, float y, float z);
		Vector3();
		Vector3(const Vector3& vector3);
		virtual ~Vector3();
		void operator=(const Vector3& vector3);

		float distance() const;
		float distanceSquared() const;
		Vector3 normalize() const;
		
		Vector3 crossProduct(const Vector3& v) const;

		Vector3 operator+(const Vector3& v) const;
		Vector3 operator-(const Vector3& v) const;
		Vector3 operator*(const float& f) const;
		Vector3 operator/(const float& f) const;

		void operator+=(const Vector3& v);
		void operator-=(const Vector3& v);
		void operator*=(const float& f);
		void operator/=(const float& f);
		
		inline void setZ(float z) { m_z = z; }

		inline float getZ() const { return m_z; }

		int getRoundZ() const;
		
		friend std::ostream& operator<<(std::ostream& out, Vector3 vector3);

	protected:
		float m_z;
};

} // geometry
} // flat

#endif // FLAT_GEOMETRY_VECTOR3_H



