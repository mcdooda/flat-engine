#ifndef FLAT_SHARP_AI_NAVIGATION_GRIDNAVIGATIONDATA_H
#define FLAT_SHARP_AI_NAVIGATION_GRIDNAVIGATIONDATA_H

#include "navigationdata.h"

namespace flat::sharp::ai::navigation
{

class GridNavigationData : public NavigationData
{
	public:
		virtual void reserveCells(int cellsCount) = 0;

		virtual AreaId createCell(const Vector2i& xy, float z, Navigability navigability) = 0;
		virtual void deleteCell(AreaId cellIndex) = 0;

		virtual AreaId findCellIndex(const Vector2& position) const = 0;
		virtual AreaId findCellIndexIfNavigable(const Vector2& position, Navigability navigabilityMask) const = 0;

		virtual void setCellZ(AreaId cellIndex, float z) = 0;
		virtual void setCellNavigability(AreaId cellIndex, Navigability navigability) = 0;
		virtual bool isCellNavigable(AreaId cellIndex, Navigability navigabilityMask) const = 0;

		virtual const Vector2i& getCellXY(AreaId cellIndex) const = 0;
		virtual float getCellZ(AreaId cellIndex) const = 0;
		virtual Navigability getCellNavigability(AreaId cellIndex) const = 0;
};

} // flat::sharp::ai::navigation

#endif // FLAT_SHARP_AI_NAVIGATION_GRIDNAVIGATIONDATA_H
