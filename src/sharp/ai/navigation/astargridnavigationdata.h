#ifndef FLAT_SHARP_AI_NAVIGATION_ASTARGRIDNAVIGATIONDATA_H
#define FLAT_SHARP_AI_NAVIGATION_ASTARGRIDNAVIGATIONDATA_H

#include <array>
#include <unordered_map>
#include <unordered_set>

#include "sharp/ai/navigation/gridnavigationdata.h"

namespace flat::sharp::ai::navigation
{

struct Cell
{
	flat::Vector2i xy;
	float z;
	Navigability navigability;
	
	static constexpr int MAX_NEIGHBORS = 4;
	std::array<AreaId, MAX_NEIGHBORS> neighbors;
};

class AStarGridNavigationData final : public GridNavigationData
{
	protected:
		struct Node
		{
			AreaId cellIndex;
			float distance;
			float heuristic;
			bool operator<(const Node& other) const { return heuristic < other.heuristic; }
			bool operator==(const Node& other) const { return cellIndex == other.cellIndex; }
		};

	public:
		virtual void reserveCells(int cellsCount) override;

		virtual AreaId createCell(const Vector2i& xy, float z, Navigability navigability) override;
		virtual void deleteCell(AreaId cellIndex) override;

		virtual AreaId findCellIndex(const Vector2& position) const override;
		virtual AreaId findCellIndexIfNavigable(const Vector2& position, Navigability navigabilityMask) const override;

		virtual void setCellZ(AreaId cellIndex, float z) override;
		virtual void setCellNavigability(AreaId cellIndex, Navigability navigability) override;

		virtual const Vector2i& getCellXY(AreaId cellIndex) const override;
		virtual float getCellZ(AreaId cellIndex) const override;
		virtual Navigability getCellNavigability(AreaId cellIndex) const override;
		virtual bool isCellNavigable(AreaId cellIndex, Navigability navigabilityMask) const override;

		virtual Result findPath(const Query& query, Path& path) const override;
		virtual void navigationRaycast(const RaycastQuery& query, RaycastResult& result) const override;

		virtual bool isDirty() const override;
		virtual void updateDirtyAreas() override;
		virtual void updateAllAreas() override;
		virtual void clear() override;

	protected:
		Result AStarSearch(const Query& query, Path& path) const;

		void buildStraightPath(const Query& query, Path& path) const;

		void reconstructPath(const Query& query, const std::unordered_map<AreaId, AreaId>& previous, const AreaId lastIndex, Result pathfindingResult, Path& path) const;
		void optimizePath(const Query& query, Path& path) const;

		void setCellDirty(AreaId cellIndex);
		bool isCellDirty(AreaId cellIndex) const;

		void clearCellNeighbors(AreaId cellIndex);
		void addCellNeighbor(AreaId cellIndex, AreaId neighborCellIndex);
		bool addCellNeighbor(AreaId cellIndex, const Vector2i& neighborRelativePosition);

	private:
		std::vector<Cell> m_cells;
		std::unordered_map<flat::Vector2i, AreaId> m_cellPositionToIndex;

		std::unordered_set<AreaId> m_dirtyCells;
		bool m_isInitialized = false;
};

} // flat::sharp::ai::navigation

#endif // FLAT_SHARP_AI_NAVIGATION_ASTARGRIDNAVIGATIONDATA_H
