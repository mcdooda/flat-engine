#ifndef FLAT_GEOMETRY_GRID_H
#define FLAT_GEOMETRY_GRID_H

#include <vector>

#include "geometry/quadtree.h"

#include "misc/aabb2.h"

namespace flat
{
namespace geometry
{

template <class T>
using GridCell = QuadTreeCell<T>;

template <class T, int width, int height>
class Grid
{
	private:
		using Cell = GridCell<T>;

	public:
		Grid(const AABB2& aabb);
		~Grid() = default;

		int addObject(const T* object);
		int updateObject(const T* object, int previousCellIndex);
		void removeObject(const T* object);
		void removeObject(const T* object, int cellIndex);
		void updateAllObjects();
		void updateAllObjects(std::unordered_map<const T*, int>& objectCellIndices);
		void clear();
		void getObjects(const AABB2& aabb, std::vector<const T*>& objects) const;
		void getObjects(const Vector2& point, std::vector<const T*>& objects) const;

	private:
		int  getCellIndex(const Cell& cell) const;
		Cell& getCell(int cellX, int cellY);
		const Cell& getCell(int cellX, int cellY) const;
		Cell& findCellAroundPoint(const Vector2& point);
		const Cell& findCellAroundPoint(const Vector2& point) const;
		void getCellPosition(const Cell& cell, int& cellX, int& cellY) const;
		void updateCellObjects(Cell& cell);

		static constexpr int getNumCells();

	private:
		static constexpr int NUM_CELLS = getNumCells();
		std::array<Cell, NUM_CELLS> m_cells;
		AABB2 m_aabb;
		Vector2 m_cellSize;
};

template<class T, int width, int height>
inline Grid<T, width, height>::Grid(const AABB2& aabb) :
	m_aabb(aabb)
{
	AABB2 cellAABB;
	const float cellWidth = aabb.getSize().x;
	const float cellHeight = aabb.getSize().x;
	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			Cell& cell = getCell(x, y);
			cellAABB.min.x = cellWidth * x;
			cellAABB.max.x = cellAABB.min.x + cellWidth;
			cellAABB.min.y = cellHeight * y;
			cellAABB.max.y = cellAABB.min.y + cellHeight;
			cell.setAABB(cellAABB);
		}
	}
	m_cellSize.x = cellWidth;
	m_cellSize.y = cellHeight;
}

template<class T, int width, int height>
inline int Grid<T, width, height>::addObject(const T* object)
{
	const AABB2& aabb = object->getAABB();
	Cell& cell = findCellAroundPoint(aabb.getCenter());
	cell.addObject(object);
	return getCellIndex(cell);
}

template<class T, int width, int height>
inline int Grid<T, width, height>::updateObject(const T* object, int previousCellIndex)
{
	Cell& previousCell = m_cells[previousCellIndex];
	const AABB2& aabb = object->getAABB();
	Cell& newCell = findCellAroundPoint(aabb.getCenter());
	if (&newCell != &previousCell)
	{
		previousCell.removeObject(object);
		newCell.addObject(object);
	}
	return getCellIndex(newCell);
}

template<class T, int width, int height>
inline void Grid<T, width, height>::removeObject(const T* object)
{
	const AABB2& aabb = object->getAABB();
	Cell& cell = findCellAroundPoint(aabb.getCenter());
	cell.removeObject(object);
}

template<class T, int width, int height>
inline void Grid<T, width, height>::removeObject(const T* object, int cellIndex)
{
	m_cells[cellIndex].removeObject(object);
}

template<class T, int width, int height>
inline void Grid<T, width, height>::updateAllObjects()
{
	for (int i = 0; i < NUM_CELLS; ++i)
	{
		Cell& cell = m_cells[i];
		updateCellObjects(cell);
	}
}

template<class T, int width, int height>
inline void Grid<T, width, height>::updateAllObjects(std::unordered_map<const T*, int>& objectCellIndices)
{
	updateAllObjects();
	for (int i = 0; i < NUM_CELLS; ++i)
	{
		Cell& cell = m_cells[i];
		for (const T* object : cell.getObjects())
		{
			objectCellIndices[object] = i;
		}
	}
}

template<class T, int width, int height>
inline void Grid<T, width, height>::clear()
{
	for (Cell<T>& cell : m_cells)
	{
		cell.clear();
	}
}

template<class T, int width, int height>
inline void Grid<T, width, height>::getObjects(const AABB2& aabb, std::vector<const T*>& objects) const
{
	const float cellHalfWidth = m_cellSize.x / 2.f;
	const float cellHalfHeight = m_cellSize.y / 2.f;

	Vector2 min = aabb.min;
	if (min.x < m_aabb.min.x)
	{
		min.x = m_aabb.min.x;
	}
	else if (min.x > m_aabb.min.x + cellHalfWidth)
	{
		min.x -= cellHalfWidth;
	}

	if (min.y < m_aabb.min.y)
	{
		min.y = m_aabb.min.y;
	}
	else if (min.y > m_aabb.min.y + cellHalfHeight)
	{
		min.y -= cellHalfHeight;
	}

	Vector2 max = aabb.max;
	if (max.x > m_aabb.max.x)
	{
		max.x = m_aabb.max.x;
	}
	else if (max.x < m_aabb.max.x - cellHalfWidth)
	{
		max.x += cellHalfWidth;
	}

	if (max.y > m_aabb.max.y)
	{
		max.y = m_aabb.max.y;
	}
	else if (max.y < m_aabb.max.y - cellHalfHeight)
	{
		max.y += cellHalfHeight;
	}

	const Cell& bottomLeftCell = findCellAroundPoint(min);
	const Cell& topRightCell = findCellAroundPoint(max);

	int minCellX, minCellY, maxCellX, maxCellY;
	getCellPosition(bottomLeftCell, minCellX, minCellY);
	getCellPosition(topRightCell, maxCellX, maxCellY);

	for (int x = minCellX; x <= maxCellX; ++x)
	{
		for (int y = minCellY; y <= maxCellY; ++y)
		{
			const Cell& cell = getCell(x, y);
			for (const T* object : cell.getObjects())
			{
				if (AABB2::overlap(aabb, object->getAABB()))
				{
					objects.push_back(object);
				}
			}
		}
	}
}

template<class T, int width, int height>
inline void Grid<T, width, height>::getObjects(const Vector2& point, std::vector<const T*>& objects) const
{
	const float cellHalfWidth = m_cellSize.x / 2.f;
	const float cellHalfHeight = m_cellSize.y / 2.f;

	Vector2 min = point;
	if (min.x < m_aabb.min.x)
	{
		min.x = m_aabb.min.x;
	}
	else if (min.x > m_aabb.min.x + cellHalfWidth)
	{
		min.x -= cellHalfWidth;
	}

	if (min.y < m_aabb.min.y)
	{
		min.y = m_aabb.min.y;
	}
	else if (min.y > m_aabb.min.y + cellHalfHeight)
	{
		min.y -= cellHalfHeight;
	}

	Vector2 max = point;
	if (max.x > m_aabb.max.x)
	{
		max.x = m_aabb.max.x;
	}
	else if (max.x < m_aabb.max.x - cellHalfWidth)
	{
		max.x += cellHalfWidth;
	}

	if (max.y > m_aabb.max.y)
	{
		max.y = m_aabb.max.y;
	}
	else if (max.y < m_aabb.max.y - cellHalfHeight)
	{
		max.y += cellHalfHeight;
	}

	const Cell& bottomLeftCell = findCellAroundPoint(min);
	const Cell& topRightCell = findCellAroundPoint(max);

	int minCellX, minCellY, maxCellX, maxCellY;
	getCellPosition(bottomLeftCell, minCellX, minCellY);
	getCellPosition(topRightCell, maxCellX, maxCellY);

	for (int x = minCellX; x <= maxCellX; ++x)
	{
		for (int y = minCellY; y <= maxCellY; ++y)
		{
			const Cell& cell = getCell(x, y);
			for (const T* object : cell.getObjects())
			{
				if (object->getAABB().isInside(point))
				{
					objects.push_back(object);
				}
			}
		}
	}
}

template<class T, int width, int height>
inline int Grid<T, width, height>::getCellIndex(const Cell& cell) const
{
	return static_cast<int>(&cell - &m_cells[0]);
}

template<class T, int width, int height>
inline GridCell<T>& Grid<T, width, height>::getCell(int cellX, int cellY)
{
	return m_cells[cellX + width * cellY];
}

template<class T, int width, int height>
inline const GridCell<T>& Grid<T, width, height>::getCell(int cellX, int cellY) const
{
	return m_cells[cellX + width * cellY];
}

template<class T, int width, int height>
inline GridCell<T>& Grid<T, width, height>::findCellAroundPoint(const Vector2& point)
{
	Vector2 gridSize = m_aabb.getSize();
	int cellX = static_cast<int>((point.x - m_aabb.min.x) / gridSize.x);
	int cellY = static_cast<int>((point.y - m_aabb.min.y) / gridSize.y);
	return getCell(cellX, cellY);
}

template<class T, int width, int height>
inline const GridCell<T>& Grid<T, width, height>::findCellAroundPoint(const Vector2& point) const
{
	Vector2 gridSize = m_aabb.getSize();
	int cellX = static_cast<int>((point.x - m_aabb.min.x) / gridSize.x);
	int cellY = static_cast<int>((point.y - m_aabb.min.y) / gridSize.y);
	return getCell(cellX, cellY);
}

template<class T, int width, int height>
inline void Grid<T, width, height>::getCellPosition(const Cell& cell, int& cellX, int& cellY) const
{
	int cellIndex = getCellIndex(cell);
	cellX = cellIndex % width;
	cellY = cellIndex / width;
}

template<class T, int width, int height>
inline void Grid<T, width, height>::updateCellObjects(Cell& cell)
{
	std::vector<const T*>& cellObjects = cell.getObjects();
	for (std::vector<const T*>::iterator it = cellObjects.begin(); it != cellObjects.end();)
	{
		const T* object = *it;
		const AABB2& aabb = object->getAABB();
		Cell& newCell = findCellAroundPoint(aabb.getCenter());
		if (&newCell != &cell)
		{
			it = cellObjects.erase(it);
			newCell.addObject(object);
		}
		else
		{
			++it;
		}
	}
}

template<class T, int width, int height>
inline constexpr int Grid<T, width, height>::getNumCells()
{
	return width * height;
}

} //geometry
} // flat

#endif // FLAT_GEOMETRY_GRID_H


