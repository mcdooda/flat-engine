#ifndef FLAT_GEOMETRY_VECTOR2_H
#define FLAT_GEOMETRY_VECTOR2_H

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
using glm::normalize;
using glm::dot;
using glm::cross;

const float PI = glm::pi<float>();

} // flat

#endif // FLAT_GEOMETRY_VECTOR2_H


