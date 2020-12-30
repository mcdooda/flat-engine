#ifndef FLAT_SHARP_AI_NAVIGATION_NAVIGATIONDATA_H
#define FLAT_SHARP_AI_NAVIGATION_NAVIGATIONDATA_H

#include "util/convertible.h"
#include "misc/vector.h"
#include "sharp/ai/navigation/path.h"
#include <functional>

namespace flat::sharp::ai::navigation
{

class NavigationData;

enum AreaId
{
	INVALID_VALUE = 0xFFFFFFFF
};

inline bool isValidArea(AreaId areaId)
{
	return areaId != AreaId::INVALID_VALUE;
}

enum Navigability : unsigned char
{
	NONE = 0,
	GROUND = 1 << 0,
	WATER = 1 << 1,
	ALL = 0xFF
};

struct Query
{
	static constexpr int DEFAULT_ITERATION_LIMIT = 1500;

	// optional filter to allow or prevent area traversal depending on custom rules
	std::function<bool(const NavigationData&, AreaId)> areaFilter;

	Vector2 from;
	Vector2 to;
	float jumpHeight;
	Navigability navigabilityMask;
	int iterationLimit = DEFAULT_ITERATION_LIMIT;
	bool allowPartialResult = false;
	bool shouldOptimizePath = true;
};

enum class Result : std::uint8_t
{
	SUCCESS,
	PARTIAL,
	FAILURE
};

struct RaycastQuery
{
	Vector2 startPosition;
	Vector2 direction;
	float length;
	float jumpHeight;
	Navigability navigabilityMask;
};

struct RaycastResult 
{
	Vector2 endPosition;
	bool didHit;
};

class NavigationData : public util::Convertible<NavigationData>
{
	public:
		virtual Result findPath(const Query& query, Path& path) const = 0;
		virtual void navigationRaycast(const RaycastQuery& query, RaycastResult& result) const = 0;
		virtual bool straightPathExists(const Query& query) const;

		virtual bool isDirty() const = 0;
		virtual void updateDirtyAreas() = 0;
		virtual void updateAllAreas() = 0;
		virtual void clear() = 0;

	protected:
		static void pathfindingToRaycastQuery(const Query& query, RaycastQuery& raycastQuery);
};

} // flat::sharp::ai::navigation

#endif // FLAT_SHARP_AI_NAVIGATION_NAVIGATIONDATA_H