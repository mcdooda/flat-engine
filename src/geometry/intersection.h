#ifndef FLAT_GEOMETRY_INTERSECTION_H
#define FLAT_GEOMETRY_INTERSECTION_H

#include "misc/vector.h"
#include "misc/aabb2.h"

namespace flat
{
namespace geometry
{
namespace intersection
{

float rectangleToRectangleDistance(const flat::AABB2& a, const flat::AABB2& b, flat::Vector2* direction = nullptr);

flat::Vector2 closestPointOnRectangle(const flat::AABB2& rectangle, const flat::Vector2& point, bool* isInside = nullptr);
float circleToRectangleDistance(const flat::AABB2& rectangle, const flat::Vector2& circleCenter, float circleRadius, flat::Vector2* direction = nullptr);

bool twoLineSegments(const flat::Vector2& start1, const flat::Vector2& end1, const flat::Vector2& start2, const flat::Vector2& end2, flat::Vector2& intersection);

} // intersection
} // geometry
} // flat

#endif // FLAT_GEOMETRY_INTERSECTION_H