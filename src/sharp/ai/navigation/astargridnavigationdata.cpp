#include <unordered_map>
#include <unordered_set>

#include "astargridnavigationdata.h"

#include "profiler/profiler.h"
#include "profiler/profilersection.h"

namespace flat::sharp::ai::navigation
{

void AStarGridNavigationData::reserveCells(int cellsCount)
{
	m_cells.reserve(cellsCount);
}

AreaId AStarGridNavigationData::createCell(const Vector2i& xy, float z, Navigability navigability)
{
	Cell& cell = m_cells.emplace_back();
	const AreaId cellIndex = static_cast<AreaId>(m_cells.size() - 1);

	cell.xy = xy;
	cell.z = z;
	cell.navigability = navigability;
	clearCellNeighbors(cellIndex);

	FLAT_ASSERT(m_cellPositionToIndex.find(xy) == m_cellPositionToIndex.end());
	m_cellPositionToIndex[xy] = cellIndex;

	if (m_isInitialized)
	{
		setCellDirty(cellIndex);
	}

	return cellIndex;
}

void AStarGridNavigationData::deleteCell(AreaId cellIndex)
{
	for (const AreaId neighborCellIndex : m_cells.at(cellIndex).neighbors)
	{
		if (!isValidArea(neighborCellIndex))
		{
			break;
		}

		setCellDirty(neighborCellIndex);
	}
	m_dirtyCells.erase(cellIndex);

	const AreaId movedCellIndex = static_cast<AreaId>(m_cells.size() - 1);
	const Vector2i deletedCellXY = m_cells.at(cellIndex).xy;
	if (cellIndex != movedCellIndex)
	{
		m_cells.at(cellIndex) = std::move(m_cells.at(movedCellIndex));
		const Vector2i movedCellXY = m_cells.at(movedCellIndex).xy;
		m_cellPositionToIndex.at(movedCellXY) = cellIndex;
	}
	m_cellPositionToIndex.erase(deletedCellXY);
	m_cells.pop_back();
}

AreaId AStarGridNavigationData::findCellIndex(const Vector2& position) const
{
	const Vector2i cellPosition(std::round(position.x), std::round(position.y));
	std::unordered_map<flat::Vector2i, AreaId>::const_iterator it = m_cellPositionToIndex.find(cellPosition);
	if (it != m_cellPositionToIndex.cend())
	{
		const AreaId cellIndex = it->second;
		FLAT_ASSERT(isValidArea(cellIndex) && cellIndex < m_cells.size());
		return cellIndex;
	}
	return AreaId::INVALID_VALUE;
}

AreaId AStarGridNavigationData::findCellIndexIfNavigable(const Vector2& position, Navigability navigabilityMask) const
{
	const AreaId cellIndex = findCellIndex(position);
	if (isValidArea(cellIndex) && isCellNavigable(cellIndex, navigabilityMask))
	{
		return cellIndex;
	}
	return AreaId::INVALID_VALUE;
}

void AStarGridNavigationData::setCellZ(AreaId cellIndex, float z)
{
	FLAT_ASSERT(isValidArea(cellIndex) && cellIndex < m_cells.size());
	m_cells.at(cellIndex).z = z;
}

void AStarGridNavigationData::setCellNavigability(AreaId cellIndex, Navigability navigability)
{
	FLAT_ASSERT(isValidArea(cellIndex) && cellIndex < m_cells.size());
	m_cells.at(cellIndex).navigability = navigability;
}

const flat::Vector2i& AStarGridNavigationData::getCellXY(AreaId cellIndex) const
{
	return m_cells.at(cellIndex).xy;
}

float AStarGridNavigationData::getCellZ(AreaId cellIndex) const
{
	return m_cells.at(cellIndex).z;
}

Navigability AStarGridNavigationData::getCellNavigability(AreaId cellIndex) const
{
	return m_cells.at(cellIndex).navigability;
}

bool AStarGridNavigationData::isCellNavigable(AreaId cellIndex, Navigability navigabilityMask) const
{
	FLAT_ASSERT(isValidArea(cellIndex) && cellIndex < m_cells.size());

	if (navigabilityMask == NONE)
	{
		return true;
	}
	return (m_cells.at(cellIndex).navigability & navigabilityMask) != 0;
}

Result AStarGridNavigationData::findPath(const Query& query, Path& path) const
{
	FLAT_ASSERT(m_isInitialized && !isDirty());

	if (query.shouldOptimizePath && straightPathExists(query))
	{
		buildStraightPath(query, path);
		return Result::SUCCESS;
	}
	else
	{
		return AStarSearch(query, path);
	}
}

void AStarGridNavigationData::navigationRaycast(const RaycastQuery& query, RaycastResult& result) const
{
	FLAT_PROFILE("Grid Navigation Raycast");

	if (query.length < FLT_EPSILON)
	{
		result.endPosition = query.startPosition;
		result.didHit = !isValidArea(findCellIndexIfNavigable(query.startPosition, query.navigabilityMask));
		return;
	}

	const Vector2& startPosition = query.startPosition;
	const Vector2 endPosition = startPosition + query.direction * query.length;
	const Navigability navigabilityMask = query.navigabilityMask;
	const float jumpHeight = query.jumpHeight;

	const flat::Vector2 p0 = startPosition + flat::Vector2(0.5f);
	const flat::Vector2 p1 = endPosition + flat::Vector2(0.5f);

	flat::Vector2 rd = p1 - p0;
	flat::Vector2 p = glm::floor(p0);
	flat::Vector2 rdinv(1.f / rd.x, 1.f / rd.y);
	flat::Vector2 stp = glm::sign(rd);
	flat::Vector2 delta = glm::min(rdinv * stp, 1.f);
	// start at intersection of ray with initial cell
	flat::Vector2 t_max = glm::abs((p + glm::max(stp, flat::Vector2(0.f)) - p0) * rdinv);

	float next_t = 0.f;

	float previousCellZ = 0.f;

	const AreaId startCellIndex = findCellIndex(startPosition);
	if (!isValidArea(startCellIndex) || !isCellNavigable(startCellIndex, navigabilityMask))
	{
		result.endPosition = query.startPosition;
		result.didHit = true;
		return;
	}
	else
	{
		previousCellZ = getCellZ(startCellIndex);
	}

	while (true)
	{
		const AreaId cellIndex = findCellIndex(p);
		if (!isValidArea(cellIndex) || !isCellNavigable(cellIndex, navigabilityMask))
		{
			result.endPosition = startPosition + next_t * rd;
			result.didHit = true;
			return;
		}
		else
		{
			const float cellZ = getCellZ(cellIndex);
			if (cellZ > previousCellZ + jumpHeight)
			{
				result.endPosition = startPosition + next_t * rd;
				result.didHit = true;
				return;
			}
			previousCellZ = cellZ;
		}

		next_t = glm::min(t_max.x, t_max.y);
		if (next_t > 1.0)
		{
			break;
		}

		const Vector2 cmp = glm::step(t_max, Vector2(t_max.y, t_max.x));
		t_max += delta * cmp;
		p += stp * cmp;
	}

	result.endPosition = endPosition;
	result.didHit = false;
}

bool AStarGridNavigationData::isDirty() const
{
	return !m_dirtyCells.empty();
}

void AStarGridNavigationData::updateDirtyAreas()
{
	FLAT_PROFILE("Grid Update Dirty Areas");

	if (!isDirty())
	{
		return;
	}

	for (const AreaId cellIndex : m_dirtyCells)
	{
		const Cell& cell = m_cells.at(cellIndex);
		const flat::Vector2i& cellXY = cell.xy;

		clearCellNeighbors(cellIndex);
		addCellNeighbor(cellIndex, cellXY + flat::Vector2i(0, -1));
		addCellNeighbor(cellIndex, cellXY + flat::Vector2i(-1, 0));
		addCellNeighbor(cellIndex, cellXY + flat::Vector2i(1, 0));
		addCellNeighbor(cellIndex, cellXY + flat::Vector2i(0, 1));

		for (const AreaId neighborCellIndex : cell.neighbors)
		{
			if (!isValidArea(neighborCellIndex))
			{
				break;
			}

			if (isCellDirty(neighborCellIndex))
			{
				// this cell has already been updated earlier or will be updated later during this loop
				continue;
			}

			addCellNeighbor(neighborCellIndex, cellIndex);
		}
	}

	m_dirtyCells.clear();
	FLAT_ASSERT(!isDirty());
}

void AStarGridNavigationData::updateAllAreas()
{
	FLAT_PROFILE("Grid Update All Areas");

	for (const std::pair<flat::Vector2i, AreaId>& pair : m_cellPositionToIndex)
	{
		const flat::Vector2i& cellXY = pair.first;
		const AreaId cellIndex = pair.second;

		clearCellNeighbors(cellIndex);
		addCellNeighbor(cellIndex, cellXY + flat::Vector2i(0, -1));
		addCellNeighbor(cellIndex, cellXY + flat::Vector2i(-1, 0));
		addCellNeighbor(cellIndex, cellXY + flat::Vector2i(1, 0));
		addCellNeighbor(cellIndex, cellXY + flat::Vector2i(0, 1));
	}

	m_dirtyCells.clear();
	m_isInitialized = true;

	FLAT_ASSERT(!isDirty());
}

void AStarGridNavigationData::clear()
{
	m_cells.clear();
	m_dirtyCells.clear();
}

Result AStarGridNavigationData::AStarSearch(const Query& query, Path& path) const
{
	FLAT_PROFILE("Grid A* Search");

	path.clear();

	const Vector2& from = query.from;
	const Vector2& to = query.to;
	const Navigability navigabilityMask = query.navigabilityMask;
	const std::function<bool(const NavigationData&, AreaId)> areaFilter = query.areaFilter;

	const AreaId firstCellIndex = findCellIndexIfNavigable(from, navigabilityMask);
	if (!isValidArea(firstCellIndex))
	{
		return Result::FAILURE;
	}

	const AreaId lastCellIndex = findCellIndexIfNavigable(to, navigabilityMask);
	if (!isValidArea(lastCellIndex))
	{
		return Result::FAILURE;
	}

	if (firstCellIndex == lastCellIndex)
	{
		buildStraightPath(query, path);
		return Result::SUCCESS;
	}

	std::unordered_set<AreaId> closedList;
	std::vector<Node> openList;
	std::unordered_map<AreaId, AreaId> previous;

	Node firstNode;
	firstNode.cellIndex = firstCellIndex;
	firstNode.distance = 0.f;
	firstNode.heuristic = 0.f;

	openList.push_back(firstNode);

	int currentIteration = 0;

	while (!openList.empty())
	{
		Node current = openList.front();
		openList.erase(openList.begin());

		const AreaId cellIndex = current.cellIndex;
		closedList.insert(cellIndex);

		if (cellIndex == lastCellIndex)
		{
			constexpr Result result = Result::SUCCESS;
			reconstructPath(query, previous, cellIndex, result, path);
			return result;
		}

		if (query.allowPartialResult)
		{
			++currentIteration;
			if (currentIteration >= query.iterationLimit)
			{
				constexpr Result result = Result::PARTIAL;
				reconstructPath(query, previous, cellIndex, result, path);
				return result;
			}
		}

		const Cell& cell = m_cells.at(cellIndex);
		const float maxZ = cell.z + query.jumpHeight;
		for (const AreaId neighborCellIndex : cell.neighbors)
		{
			if (!isValidArea(neighborCellIndex))
			{
				break;
			}

			if (getCellZ(neighborCellIndex) > maxZ)
			{
				continue;
			}
			else if (!isCellNavigable(neighborCellIndex, navigabilityMask))
			{
				continue;
			}
			else if (areaFilter && !areaFilter(*this, neighborCellIndex))
			{
				continue;
			}

			if (closedList.count(neighborCellIndex) > 0)
			{
				// this cell has already been visited
				continue;
			}

			Node neighbor;
			neighbor.cellIndex = neighborCellIndex;
			neighbor.distance = current.distance + 1.f;
			const flat::Vector2i& xy = m_cells.at(neighborCellIndex).xy;
			const float estimatedDistance = flat::distance(to, flat::Vector2(xy));
			neighbor.heuristic = neighbor.distance + estimatedDistance;

			const std::vector<Node>::iterator it = std::find(openList.begin(), openList.end(), neighbor);
			if (it == openList.end())
			{
				// sorted insertion to mimic a priority queue
				const std::vector<Node>::iterator it = std::upper_bound(openList.begin(), openList.end(), neighbor);
				openList.insert(it, neighbor);
				previous.emplace(neighborCellIndex, cellIndex);
			}
			else if (neighbor.distance < it->distance)
			{
				// update distance
				it->distance = neighbor.distance;
				it->heuristic = neighbor.heuristic;
				previous.emplace(neighborCellIndex, cellIndex);
			}
		}
	}

	return Result::FAILURE;
}

void AStarGridNavigationData::buildStraightPath(const Query& query, Path& path) const
{
	path.clear();
	path.appendPoint(query.from);
	path.appendPoint(query.to);
}

void AStarGridNavigationData::reconstructPath(const Query& query, const std::unordered_map<AreaId, AreaId>& previous, const AreaId lastIndex, Result pathfindingResult, Path& path) const
{
	FLAT_ASSERT(pathfindingResult != Result::FAILURE);

	path.clear();

	std::unordered_map<AreaId, AreaId>::const_iterator it;
	AreaId currentIndex = lastIndex;
	while (true)
	{
		it = previous.find(currentIndex);
		if (it != previous.end())
		{
			currentIndex = it->second;
			const flat::Vector2i& xy = m_cells.at(currentIndex).xy;
			path.prependPoint(flat::Vector2(xy));
		}
		else
		{
			break;
		}
	}

	FLAT_ASSERT(!path.isEmpty());

	path.setPoint(0, query.from);
	if (pathfindingResult == Result::SUCCESS)
	{
		path.appendPoint(query.to);
	}

	if (query.shouldOptimizePath)
	{
		optimizePath(query, path);
	}

	FLAT_ASSERT(!path.isEmpty());
}

void AStarGridNavigationData::optimizePath(const Query& query, Path& path) const
{
	FLAT_PROFILE("Grid Optimize Path");

	// TODO: use a binary approach instead

	int i = path.getPointsCount() - 1;

	Query straightPathQuery = query;

	while (i >= 2)
	{
		while (i >= 2)
		{
			straightPathQuery.from = path.getPoint(i - 2);
			straightPathQuery.to = path.getPoint(i);
			if (!straightPathExists(straightPathQuery))
			{
				break;
			}

			path.removePoint(i);
			--i;
		}
		--i;
	}
}

void AStarGridNavigationData::setCellDirty(AreaId cellIndex)
{
	FLAT_ASSERT(isValidArea(cellIndex) && cellIndex < m_cells.size());
	m_dirtyCells.insert(cellIndex);
}

bool AStarGridNavigationData::isCellDirty(AreaId cellIndex) const
{
	return m_dirtyCells.count(cellIndex) > 0;
}

void AStarGridNavigationData::clearCellNeighbors(AreaId cellIndex)
{
	FLAT_ASSERT(isValidArea(cellIndex) && cellIndex < m_cells.size());

	m_cells.at(cellIndex).neighbors[0] = AreaId::INVALID_VALUE;
#ifdef FLAT_DEBUG
	m_cells.at(cellIndex).neighbors.fill(AreaId::INVALID_VALUE);
#endif
}

void AStarGridNavigationData::addCellNeighbor(AreaId cellIndex, AreaId neighborCellIndex)
{
	FLAT_ASSERT(isValidArea(cellIndex) && cellIndex < m_cells.size());

	std::array<AreaId, Cell::MAX_NEIGHBORS>& neighbors = m_cells.at(cellIndex).neighbors;
	for (size_t i = 0; i < Cell::MAX_NEIGHBORS; ++i)
	{
		if (!isValidArea(neighbors[i]))
		{
			neighbors[i] = neighborCellIndex;

			// reorder by ascending order
			for (size_t j = i; j > 0 && neighbors[j] < neighbors[j - 1]; --j)
			{
				std::swap(neighbors[j], neighbors[j - 1]);
			}
			return;
		}
	}
	FLAT_ASSERT_MSG(false, "Cannot add more than %d neighbors", Cell::MAX_NEIGHBORS);
}

bool AStarGridNavigationData::addCellNeighbor(AreaId cellIndex, const Vector2i& neighborPosition)
{
	FLAT_ASSERT(isValidArea(cellIndex) && cellIndex < m_cells.size());

	const AreaId neighborCellIndex = findCellIndex(neighborPosition);
	if (isValidArea(neighborCellIndex))
	{
		addCellNeighbor(cellIndex, neighborCellIndex);
		return true;
	}
	return false;
}

} // flat::sharp::ai::navigation