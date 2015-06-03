#include <cmath>
#include "vector3.h"

#include "../debug/assert.h"

namespace flat
{
namespace geometry
{

Vector3::Vector3() :
	x(0.f),
	y(0.f),
	z(0.f)
{

}

Vector3::Vector3(const Vector3& vector3) :
	x(vector3.x),
	y(vector3.y),
	z(vector3.z)
{
	
}

Vector3::Vector3(float x, float y, float z) :
	x(x),
	y(y),
	z(z)
{

}

void Vector3::operator=(const Vector3& vector3)
{
	x = vector3.x;
	y = vector3.y;
	z = vector3.z;
}

float Vector3::length() const
{
	return sqrt(x * x + y * y + z * z);
}

float Vector3::lengthSquared() const
{
	return x * x + y * y + z * z;
}

Vector3 Vector3::normalize() const
{
	float len = length();

	if (len != 0)
		return Vector3(x / len, y / len, z / len);

	else
		return *this;
}

float Vector3::dotProduct(const Vector3& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

Vector3 Vector3::crossProduct(const Vector3& v) const
{
	return Vector3(
		y * v.z - z * v.y,
		z * v.x - x * v.z,
		x * v.y - y * v.x
	);
}

Vector3 Vector3::operator+(const Vector3& v) const
{
	return Vector3(x + v.x, y + v.y, z + v.z);
}

Vector3 Vector3::operator-(const Vector3& v) const
{
	return Vector3(x - v.x, y - v.y, z - v.z);
}

Vector3 Vector3::operator*(const float& f) const
{
	return Vector3(x * f, y * f, z * f);
}

Vector3 Vector3::operator/(const float& f) const
{
	FLAT_ASSERT(f != 0.f);
	return Vector3(x / f, y / f, z / f);
}

void Vector3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
}

void Vector3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

void Vector3::operator*=(const float& f)
{
	x *= f;
	y *= f;
	z *= f;
}

void Vector3::operator/=(const float& f)
{
	FLAT_ASSERT(f != 0.f);
	x /= f;
	y /= f;
	z /= f;
}

Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

int Vector3::getRoundX() const
{
	return round(x);
}

int Vector3::getRoundY() const
{
	return round(y);
}

int Vector3::getRoundZ() const
{
	return round(z);
}

std::ostream& operator<<(std::ostream& out, Vector3 vector3)
{
	out << "Vector3(" << vector3.x << "," << vector3.y << "," << vector3.z << ")";
	return out;
}

} // geometry
} // flat


