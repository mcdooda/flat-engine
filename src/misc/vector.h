#ifndef FLAT_MISC_VECTOR_H
#define FLAT_MISC_VECTOR_H

#include <ostream>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include "../debug/assert.h"
#include "math.h"

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

#endif // FLAT_MISC_VECTOR_H


