#include <cmath>
#include "vector3.h"

namespace flat
{
namespace geometry
{

Vector3::Vector3(float x, float y, float z) : Vector2(x, y),
	m_z(z)
{

}

Vector3::Vector3() :
	m_z(0)
{

}

Vector3::Vector3(const Vector3& Vector3)
{
	*this = Vector3;
}

Vector3::~Vector3()
{
	
}

void Vector3::operator=(const Vector3& Vector3)
{
	m_x = Vector3.m_x;
	m_y = Vector3.m_y;
	m_z = Vector3.m_z;
}

float Vector3::distance() const
{
	return sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
}

float Vector3::distanceSquared() const
{
	return m_x * m_x + m_y * m_y + m_z * m_z;
}

Vector3 Vector3::normalize() const
{
	float d = distance();

	if (d != 0)
		return Vector3(m_x / d, m_y / d, m_z / d);

	else
		return *this;
}

Vector3 Vector3::crossProduct(const Vector3& v) const
{
	return Vector3(
		m_y * v.m_z - m_z * v.m_y,
		m_z * v.m_x - m_x * v.m_z,
		m_x * v.m_y - m_y * v.m_x
	);
}

Vector3 Vector3::operator+(const Vector3& v) const
{
	return Vector3(m_x + v.m_x, m_y + v.m_y, m_z + v.m_z);
}

Vector3 Vector3::operator-(const Vector3& v) const
{
	return Vector3(m_x - v.m_x, m_y - v.m_y, m_z - v.m_z);
}

Vector3 Vector3::operator*(const float& f) const
{
	return Vector3(m_x * f, m_y * f, m_z * f);
}

Vector3 Vector3::operator/(const float& f) const
{
	if (f != 0)
		return Vector3(m_x / f, m_y / f, m_z / f);

	else
		return *this;
}

void Vector3::operator+=(const Vector3& v)
{
	m_x += v.m_x;
	m_y += v.m_y;
	m_z += v.m_z;
}

void Vector3::operator-=(const Vector3& v)
{
	m_x -= v.m_x;
	m_y -= v.m_y;
	m_z -= v.m_z;
}

void Vector3::operator*=(const float& f)
{
	m_x *= f;
	m_y *= f;
	m_z *= f;
}

void Vector3::operator/=(const float& f)
{
	if (f != 0)
	{
		m_x /= f;
		m_y /= f;
		m_z /= f;
	}
}

int Vector3::getRoundZ() const
{
	return round(m_z);
}

std::ostream& operator<<(std::ostream& out, Vector3 vector3)
{
	out << "Vector3(" << vector3.m_x << "," << vector3.m_y << "," << vector3.m_z << ")";
	return out;
}

} // geometry
} // flat


