#ifndef FLAT_MISC_VECTOR_H
#define FLAT_MISC_VECTOR_H

#include <ostream>
#include <cmath>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include "misc/math.h"

#include "debug/assert.h"

namespace flat
{

using Vector2 = glm::vec2;
using Vector3 = glm::vec3;
using Vector4 = glm::vec4;

using Vector2i = glm::ivec2;
using Vector3i = glm::ivec3;
using Vector4i = glm::ivec4;

inline float vector2_angle(const Vector2& vector2)
{
	FLAT_ASSERT(vector2.x != 0 || vector2.y != 0);
	return std::atan2(vector2.y, vector2.x);
}

template <class T>
inline T normalize(const T& vector)
{
	if (glm::length2(vector) == 0)
		return vector;

	return glm::normalize(vector);
}

using glm::length;
using glm::length2;
using glm::distance;
using glm::distance2;
using glm::dot;
using glm::cross;
using glm::reflect;

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

namespace std
{
	template <>
	struct hash<flat::Vector2i>
	{
		size_t operator()(const flat::Vector2i& k) const
		{
			return static_cast<size_t>(k.x) * 0x9e3779b9 + k.y;
		}
	};
}

#endif // FLAT_MISC_VECTOR_H


