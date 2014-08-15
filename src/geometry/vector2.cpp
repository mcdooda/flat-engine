#include <cmath>
#include "vector2.h"

namespace flat
{
namespace geometry
{

Vector2::Vector2(float x, float y) :
	m_x(x),
	m_y(y)
{

}

Vector2::Vector2(float alpha)
{
	m_x = cos(alpha);
	m_y = sin(alpha);
}

Vector2::Vector2() :
	m_x(0),
	m_y(0)
{

}

Vector2::Vector2(const Vector2& vector2)
{
	*this = vector2;
}

void Vector2::operator=(const Vector2& vector2)
{
	m_x = vector2.m_x;
	m_y = vector2.m_y;
}

float Vector2::distance() const
{
	return sqrt(m_x * m_x + m_y * m_y);
}

float Vector2::distanceSquared() const
{
	return m_x * m_x + m_y * m_y;
}

float Vector2::angle() const
{
	float angle = atan(m_y / m_x);

	if (m_x < 0)
		angle += M_PI;

	return angle;
}

Vector2 Vector2::normalize() const
{
	float d = distance();

	if (d != 0)
		return Vector2(m_x / d, m_y / d);

	else
		return *this;
}

Vector2 Vector2::operator+(const Vector2& v) const
{
	return Vector2(m_x + v.m_x, m_y + v.m_y);
}

Vector2 Vector2::operator-(const Vector2& v) const
{
	return Vector2(m_x - v.m_x, m_y - v.m_y);
}

Vector2 Vector2::operator*(const float& f) const
{
	return Vector2(m_x * f, m_y * f);
}

Vector2 Vector2::operator/(const float& f) const
{
	if (f != 0)
		return Vector2(m_x / f, m_y / f);

	else
		return *this;
}

void Vector2::operator+=(const Vector2& v)
{
	m_x += v.m_x;
	m_y += v.m_y;
}

void Vector2::operator-=(const Vector2& v)
{
	m_x -= v.m_x;
	m_y -= v.m_y;
}

void Vector2::operator*=(const float& f)
{
	m_x *= f;
	m_y *= f;
}

void Vector2::operator/=(const float& f)
{
	if (f != 0)
	{
		m_x /= f;
		m_y /= f;
	}
}

int Vector2::getRoundX() const
{
	return round(m_x);
}

int Vector2::getRoundY() const
{
	return round(m_y);
}

std::ostream& operator<<(std::ostream& out, Vector2 vector2)
{
	out << "(" << vector2.m_x << "," << vector2.m_y << ")";
	return out;
}

} // geometry
} // flat


