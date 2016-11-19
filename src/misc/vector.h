#ifndef FLAT_GEOMETRY_VECTOR2_H
#define FLAT_GEOMETRY_VECTOR2_H

#include <ostream>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include "../debug/assert.h"

namespace flat
{

using Vector2 = glm::vec2;
using Vector3 = glm::vec3;
using Vector4 = glm::vec4;

inline float vector2_angle(const Vector2& vector2)
{
	FLAT_ASSERT(vector2.x != 0 || vector2.y != 0);
	return std::atan2f(vector2.y, vector2.x);
}

using glm::length;
using glm::length2;
using glm::dot;
using glm::cross;

template <class T>
inline T normalize(const T& vector)
{
	if (glm::length2(vector) == 0)
		return vector;

	return glm::normalize(vector);
}

const float PI = glm::pi<float>();
const float PI2 = PI * 2.f;

const float SQRT2 = 1.41421356237f;

template <typename T>
int8_t sign(T number)
{
	return (T(0) < number) - (number < T(0));
}

} // flat

inline std::ostream& operator<<(std::ostream& out, const flat::Vector2& v)
{
	return out << "Vector2(" << v.x << "," << v.y << ")";
}

inline std::ostream& operator<<(std::ostream& out, const flat::Vector3& v)
{
	return out << "Vector3(" << v.x << "," << v.y << "," << v.z << ")";
}

inline std::ostream& operator<<(std::ostream& out, const flat::Vector4& v)
{
	return out << "Vector4(" << v.x << "," << v.y << "," << v.z << "," << v.w << ")";
}

#endif // FLAT_GEOMETRY_VECTOR2_H


