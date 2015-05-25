#include <cmath>
#include "vector2.h"

#include "../debug/assert.h"

namespace flat
{
namespace geometry
{

Vector2::Vector2() :
	x(0.f),
	y(0.f)
{

}

Vector2::Vector2(const Vector2& vector2) :
	x(vector2.x),
	y(vector2.y)
{
	
}

Vector2::Vector2(float x, float y) :
	x(x),
	y(y)
{

}

Vector2::Vector2(float alpha)
{
	x = cos(alpha);
	y = sin(alpha);
}

void Vector2::operator=(const Vector2& vector2)
{
	x = vector2.x;
	y = vector2.y;
}

float Vector2::length() const
{
	return sqrt(x * x + y * y);
}

float Vector2::lengthSquared() const
{
	return x * x + y * y;
}

float Vector2::angle() const
{
	FLAT_ASSERT(x != 0 || y != 0);
	return atan2f(y, x);
}

Vector2 Vector2::normalize() const
{
	float len = length();

	if (len != 0)
		return Vector2(x / len, y / len);

	else
		return *this;
}

float Vector2::dotProduct(const Vector2& v) const
{
	return x * v.x + y * v.y;
}

Vector2 Vector2::operator+(const Vector2& v) const
{
	return Vector2(x + v.x, y + v.y);
}

Vector2 Vector2::operator-(const Vector2& v) const
{
	return Vector2(x - v.x, y - v.y);
}

Vector2 Vector2::operator*(const float& f) const
{
	return Vector2(x * f, y * f);
}

Vector2 Vector2::operator/(const float& f) const
{
	FLAT_ASSERT(f != 0.f);
	return Vector2(x / f, y / f);
}

void Vector2::operator+=(const Vector2& v)
{
	x += v.x;
	y += v.y;
}

void Vector2::operator-=(const Vector2& v)
{
	x -= v.x;
	y -= v.y;
}

void Vector2::operator*=(const float& f)
{
	x *= f;
	y *= f;
}

void Vector2::operator/=(const float& f)
{
	FLAT_ASSERT(f != 0.f);
	x /= f;
	y /= f;
}

int Vector2::getRoundX() const
{
	return round(x);
}

int Vector2::getRoundY() const
{
	return round(y);
}

std::ostream& operator<<(std::ostream& out, Vector2 vector2)
{
	out << "Vector2(" << vector2.x << "," << vector2.y << ")";
	return out;
}

} // geometry
} // flat


