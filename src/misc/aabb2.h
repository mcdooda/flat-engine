#ifndef FLAT_MISC_AABB2_H
#define FLAT_MISC_AABB2_H

#include "vector.h"

namespace flat
{

class AABB2
{
	public:
		AABB2() :
			min(0.f, 0.f),
			max(0.f, 0.f)
		{}

		AABB2(const Vector2& min, const Vector2& max) :
			min(min),
			max(max)
		{}

		inline bool operator==(const AABB2& other) const { return min == other.min && max == other.max; }

		inline bool isValid() const { return min.x <= max.x && min.y <= max.y; }

		inline bool isInside(const Vector2& point) const
		{
			FLAT_ASSERT(isValid());
			return min.x <= point.x && point.x <= max.x
				&& min.y <= point.y && point.y <= max.y;
		}

		inline Vector2 getCenter() const
		{
			return (min + max) / 2.f;
		}

		inline Vector2 getSize() const
		{
			return max - min;
		}

		inline bool contains(const AABB2& other) const
		{
			return min.x <= other.min.x && min.y <= other.min.y
				&& other.max.x <= max.x && other.max.y <= max.y;
		}

		inline static bool overlap(const AABB2& a, const AABB2& b)
		{
			Vector2 aCenter = a.getCenter();
			Vector2 bCenter = b.getCenter();
			Vector2 aSize = a.getSize();
			Vector2 bSize = b.getSize();
			return (std::abs(aCenter.x - bCenter.x) * 2.f < aSize.x + bSize.x)
				&& (std::abs(aCenter.y - bCenter.y) * 2.f < aSize.y + bSize.y);
		}

	public:
		Vector2 min;
		Vector2 max;
};

} // flat

#endif // FLAT_MISC_AABB_H


