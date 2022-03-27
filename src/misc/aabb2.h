#ifndef FLAT_MISC_AABB2_H
#define FLAT_MISC_AABB2_H

#include "misc/aabb3.h"

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

		AABB2(const AABB3& aabb3) :
			min(aabb3.min),
			max(aabb3.max)
		{

		}

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

		void scaleBy(float scaleFactor)
		{
			const Vector2 scaledSize = getSize() * scaleFactor;
			const Vector2 center = getCenter();
			min = center - scaledSize;
			max = center + scaledSize;
		}

		inline bool contains(const AABB2& other) const
		{
			return min.x <= other.min.x && min.y <= other.min.y
				&& other.max.x <= max.x && other.max.y <= max.y;
		}

		inline static bool overlap(const AABB2& a, const AABB2& b)
		{
			return a.max.x > b.min.x
				&& a.min.x < b.max.x
				&& a.max.y > b.min.y
				&& a.min.y < b.max.y;
		}

	public:
		Vector2 min;
		Vector2 max;
};

} // flat

#endif // FLAT_MISC_AABB_H


