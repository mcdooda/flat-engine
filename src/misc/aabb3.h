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

		inline static bool overlap(const AABB3& a, const AABB3& b)
		{
			Vector3 aCenter = a.getCenter();
			Vector3 bCenter = b.getCenter();
			Vector3 aSize = a.getSize();
			Vector3 bSize = b.getSize();
			return (std::abs(aCenter.x - bCenter.x) * 2.f < aSize.x + bSize.x)
				&& (std::abs(aCenter.y - bCenter.y) * 2.f < aSize.y + bSize.y)
				&& (std::abs(aCenter.z - bCenter.z) * 2.f < aSize.z + bSize.z);
		}

	public:
		Vector3 min;
		Vector3 max;
};

} // flat

#endif // FLAT_MISC_AABB3_H


