#ifndef FLAT_MISC_AABB3_H
#define FLAT_MISC_AABB3_H

#include "vector.h"

namespace flat
{

class AABB3
{
	public:
		AABB3() :
			min(0.f, 0.f, 0.f),
			max(0.f, 0.f, 0.f)
		{}

		AABB3(const Vector3& min, const Vector3& max) :
			min(min),
			max(max)
		{}

		inline bool isValid() const { return min.x <= max.x && min.y <= max.y && min.z <= max.z; }

		inline bool isInside(const Vector3& point) const
		{
			FLAT_ASSERT(isValid());
			return min.x <= point.x && point.x <= max.x
				&& min.y <= point.y && point.y <= max.y
				&& min.z <= point.z && point.z <= max.z;
		}

		inline Vector3 getCenter() const
		{
			return (min + max) / 2.f;
		}

		inline Vector3 getSize() const
		{
			return max - min;
		}

		inline bool contains(const AABB3& other) const
		{
			return min.x <= other.min.x && min.y <= other.min.y && min.z <= other.min.z
				&& other.max.x <= max.x && other.max.y <= max.y && other.max.z <= max.z;
		}

		inline static bool overlap(const AABB3& a, const AABB3& b)
		{
			return a.max.x > b.min.x
				&& a.min.x < b.max.x
				&& a.max.y > b.min.y
				&& a.min.y < b.max.y
				&& a.max.z > b.min.z
				&& a.min.z < b.max.z;
		}

	public:
		Vector3 min;
		Vector3 max;
};

} // flat

#endif // FLAT_MISC_AABB3_H


