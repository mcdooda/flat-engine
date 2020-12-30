#include "navigationdata.h"

namespace flat::sharp::ai::navigation
{

bool NavigationData::straightPathExists(const Query& query) const
{
	RaycastQuery raycastQuery;
	pathfindingToRaycastQuery(query, raycastQuery);
	RaycastResult result;
	navigationRaycast(raycastQuery, result);
	return !result.didHit;
}

void NavigationData::pathfindingToRaycastQuery(const Query& query, RaycastQuery& raycastQuery)
{
	raycastQuery.startPosition = query.from;
	const Vector2 delta = query.to - query.from;
	raycastQuery.length = flat::length(delta);
	raycastQuery.direction = delta / raycastQuery.length;
	raycastQuery.jumpHeight = query.jumpHeight;
	raycastQuery.navigabilityMask = query.navigabilityMask;
}

} // flat::sharp::ai::navigation

