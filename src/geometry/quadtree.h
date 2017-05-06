#ifndef FLAT_GEOMETRY_QUADTREE_H
#define FLAT_GEOMETRY_QUADTREE_H

#include <vector>
#include "../misc/aabb2.h"
#include "../debug/helpers.h"

namespace flat
{
namespace geometry
{

template <class T>
class QuadTreeCell
{
	public:
		QuadTreeCell() = default;
		~QuadTreeCell() = default;

		inline void setAABB(const flat::AABB2& aabb) { m_aabb = aabb; }

		inline const std::vector<const T*>& getObjects() const { return m_objects; }
		inline std::vector<const T*>& getObjects() { return m_objects; }
		void addObject(const T* object);
		void removeObject(const T* object);
		void clear();

		inline bool isInside(const flat::Vector2& point) const { return m_aabb.isInside(point); }
		inline bool contains(const flat::AABB2& aabb) const { return m_aabb.contains(aabb); }
		inline bool overlap(const flat::AABB2& aabb) const { return flat::AABB2::overlap(m_aabb, aabb); }

	private:
		flat::AABB2 m_aabb;
		std::vector<const T*> m_objects;
};

template<class T>
inline void QuadTreeCell<T>::addObject(const T* object)
{
	FLAT_ASSERT(std::find(m_objects.begin(), m_objects.end(), object) == m_objects.end());
	m_objects.push_back(object);
}

template<class T>
inline void QuadTreeCell<T>::removeObject(const T* object)
{
	auto it = std::find(m_objects.begin(), m_objects.end(), object);
	FLAT_ASSERT(it != m_objects.end());
	m_objects.erase(it);
}

template<class T>
inline void QuadTreeCell<T>::clear()
{
	m_objects.clear();
}

template <class T, int depth>
class QuadTree
{
	private:
		using Cell = QuadTreeCell<T>;
		enum ChildPosition
		{
			BOTTOM_LEFT  = 1,
			BOTTOM_RIGHT = 2,
			TOP_LEFT     = 3,
			TOP_RIGHT    = 4
		};

	public:
		QuadTree(const flat::AABB2& aabb);
		~QuadTree() = default;

		int addObject(const T* object);
		int updateObject(const T* object, int previousCellIndex);
		void removeObject(const T* object);
		void removeObject(const T* object, int cellIndex);
		void updateAllObjects();
		void updateAllObjects(std::unordered_map<const T*, int>& objectCellIndices);
		void clear();
		void getObjects(const flat::AABB2& aabb, std::vector<const T*>& objects) const;

	private:
		void initAABBs(Cell& cell, const flat::AABB2& aabb);
		int  getCellIndex(const Cell& cell) const;
		bool isRoot(const Cell& cell) const;
		bool isLeaf(const Cell& cell) const;
		Cell& getRootCell();
		const Cell& getRootCell() const;
		Cell& getChild(const Cell& cell, ChildPosition childPosition);
		const Cell& getChild(const Cell& cell, ChildPosition childPosition) const;
		Cell& getParentCell(Cell& cell);
		const Cell& getParentCell(const Cell& cell) const;
		Cell& findChildCellForAABB(Cell& cell, const flat::AABB2& aabb);
		Cell& findParentCellForAABB(Cell& cell, const flat::AABB2& aabb);
		void getObjectsInCell(const Cell& cell, const flat::AABB2& aabb, std::vector<const T*>& objects) const;
		void updateCellObjects(Cell& cell);
		Cell& updateObjectCell(Cell& cell, const T* object, const flat::AABB2& aabb);

		static constexpr int cpow(int m, int n);
		static constexpr int getNumCells();

	private:
		static constexpr int NUM_CELLS = getNumCells();
		std::array<Cell, NUM_CELLS> m_cells;
};

template<class T, int depth>
inline QuadTree<T, depth>::QuadTree(const flat::AABB2& aabb)
{
	initAABBs(getRootCell(), aabb);
}

template<class T, int depth>
inline int QuadTree<T, depth>::addObject(const T* object)
{
	const flat::AABB2& aabb = object->getAABB();
	FLAT_ASSERT_MSG(getRootCell().contains(aabb), "AABB is outside of the quad tree's root cell");
	Cell& cell = findChildCellForAABB(getRootCell(), aabb);
	cell.addObject(object);
	return getCellIndex(cell);
}

template<class T, int depth>
inline int QuadTree<T, depth>::updateObject(const T* object, int previousCellIndex)
{
	Cell& previousCell = m_cells[previousCellIndex];
	const flat::AABB2& aabb = object->getAABB();
	Cell& newCell = updateObjectCell(previousCell, object, aabb);
	if (&newCell != &previousCell)
	{
		previousCell.removeObject(object);
		newCell.addObject(object);
	}
	return getCellIndex(newCell);
}

template<class T, int depth>
inline void QuadTree<T, depth>::removeObject(const T* object)
{
	const flat::AABB2& aabb = object->getAABB();
	Cell& cell = findChildCellForAABB(getRootCell(), aabb);
	cell.removeObject(object);
}

template<class T, int depth>
inline void QuadTree<T, depth>::removeObject(const T* object, int cellIndex)
{
	m_cells[cellIndex].removeObject(object);
}

template<class T, int depth>
inline void QuadTree<T, depth>::updateAllObjects()
{
	for (int i = 0; i < NUM_CELLS; ++i)
	{
		Cell& cell = m_cells[i];
		updateCellObjects(cell);
	}
}

template<class T, int depth>
inline void QuadTree<T, depth>::updateAllObjects(std::unordered_map<const T*, int>& objectCellIndices)
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

template<class T, int depth>
inline void QuadTree<T, depth>::clear()
{
	for (QuadTreeCell<T>& cell : m_cells)
	{
		cell.clear();
	}
}

template<class T, int depth>
inline void QuadTree<T, depth>::getObjects(const flat::AABB2& aabb, std::vector<const T*>& objects) const
{
	getObjectsInCell(getRootCell(), aabb, objects);
}

template<class T, int depth>
inline void QuadTree<T, depth>::initAABBs(Cell& cell, const flat::AABB2& aabb)
{
	cell.setAABB(aabb);
	if (!isLeaf(cell))
	{
		Cell& bottomLeftCell = getChild(cell, ChildPosition::BOTTOM_LEFT);
		initAABBs(bottomLeftCell, flat::AABB2(aabb.min, aabb.getCenter()));

		Cell& bottomRightCell = getChild(cell, ChildPosition::TOP_LEFT);
		initAABBs(bottomRightCell, flat::AABB2(flat::Vector2(aabb.getCenter().x, aabb.min.y), flat::Vector2(aabb.max.x, aabb.getCenter().y)));

		Cell& topLeftCell = getChild(cell, ChildPosition::TOP_LEFT);
		initAABBs(topLeftCell, flat::AABB2(flat::Vector2(aabb.min.x, aabb.getCenter().y), flat::Vector2(aabb.getCenter().x, aabb.max.y)));

		Cell& topRightCell = getChild(cell, ChildPosition::TOP_RIGHT);
		initAABBs(topRightCell, flat::AABB2(aabb.getCenter(), aabb.max));
	}
}

template<class T, int depth>
inline int QuadTree<T, depth>::getCellIndex(const Cell& cell) const
{
	return static_cast<int>(&cell - &m_cells[0]);
}

template<class T, int depth>
inline bool QuadTree<T, depth>::isRoot(const Cell& cell) const
{
	return &m_cells[0] == &cell;
}

template<class T, int depth>
inline bool QuadTree<T, depth>::isLeaf(const Cell& cell) const
{
	int index = getCellIndex(cell);
	return index * 4 + 1 >= NUM_CELLS;
}

template<class T, int depth>
inline QuadTreeCell<T>& QuadTree<T, depth>::getRootCell()
{
	return m_cells[0];
}

template<class T, int depth>
inline const QuadTreeCell<T>& QuadTree<T, depth>::getRootCell() const
{
	return m_cells[0];
}

template<class T, int depth>
inline QuadTreeCell<T>& QuadTree<T, depth>::getChild(const Cell& cell, ChildPosition childPosition)
{
	int index = getCellIndex(cell);
	return m_cells[index * 4 + childPosition];
}

template<class T, int depth>
inline const QuadTreeCell<T>& QuadTree<T, depth>::getChild(const Cell & cell, ChildPosition childPosition) const
{
	int index = getCellIndex(cell);
	return m_cells[index * 4 + childPosition];
}

template<class T, int depth>
inline QuadTreeCell<T>& QuadTree<T, depth>::getParentCell(Cell& cell)
{
	int index = getCellIndex(cell);
	return m_cells[(index - 1) / 4];
}

template<class T, int depth>
inline const QuadTreeCell<T>& QuadTree<T, depth>::getParentCell(const Cell& cell) const
{
	int index = getCellIndex(cell);
	return m_cells[(index - 1) / 4];
}

template<class T, int depth>
inline QuadTreeCell<T>& QuadTree<T, depth>::findChildCellForAABB(Cell& cell, const flat::AABB2& aabb)
{
	if (!isLeaf(cell))
	{
		Cell& bottomLeftCell = getChild(cell, ChildPosition::BOTTOM_LEFT);
		if (bottomLeftCell.contains(aabb))
		{
			return findChildCellForAABB(bottomLeftCell, aabb);
		}

		Cell& bottomRightCell = getChild(cell, ChildPosition::BOTTOM_RIGHT);
		if (bottomRightCell.contains(aabb))
		{
			return findChildCellForAABB(bottomRightCell, aabb);
		}

		Cell& topLeftCell = getChild(cell, ChildPosition::TOP_LEFT);
		if (topLeftCell.contains(aabb))
		{
			return findChildCellForAABB(topLeftCell, aabb);
		}

		Cell& topRightCell = getChild(cell, ChildPosition::TOP_RIGHT);
		if (topRightCell.contains(aabb))
		{
			return findChildCellForAABB(topRightCell, aabb);
		}
	}

	return cell;
}

template<class T, int depth>
inline QuadTreeCell<T>& QuadTree<T, depth>::findParentCellForAABB(Cell& cell, const flat::AABB2& aabb)
{
	if (cell.contains(aabb))
	{
		return findChildCellForAABB(cell, aabb);
	}
	else if (isRoot(cell))
	{
		FLAT_ASSERT_MSG(false, "AABB is outside of the quad tree's root cell");
		return cell;
	}
	else
	{
		Cell& parentCell = getParentCell(cell);
		return findParentCellForAABB(parentCell, aabb);
	}
}

template<class T, int depth>
inline void QuadTree<T, depth>::getObjectsInCell(const Cell& cell, const flat::AABB2& aabb, std::vector<const T*>& objects) const
{
	if (cell.overlap(aabb))
	{
		const std::vector<const T*> cellObjects = cell.getObjects();
		for (const T* object : cellObjects)
		{
			const flat::AABB2& objectAABB = object->getAABB();
			if (flat::AABB2::overlap(aabb, objectAABB))
			{
				objects.push_back(object);
			}
		}

		if (!isLeaf(cell))
		{
			const Cell& bottomLeftCell = getChild(cell, ChildPosition::BOTTOM_LEFT);
			getObjectsInCell(bottomLeftCell, aabb, objects);

			const Cell& bottomRightCell = getChild(cell, ChildPosition::BOTTOM_RIGHT);
			getObjectsInCell(bottomRightCell, aabb, objects);

			const Cell& topLeftCell = getChild(cell, ChildPosition::TOP_LEFT);
			getObjectsInCell(topLeftCell, aabb, objects);

			const Cell& topRightCell = getChild(cell, ChildPosition::TOP_RIGHT);
			getObjectsInCell(topRightCell, aabb, objects);
		}
	}
}

template<class T, int depth>
inline void QuadTree<T, depth>::updateCellObjects(Cell& cell)
{
	std::vector<const T*>& cellObjects = cell.getObjects();
	for (std::vector<const T*>::iterator it = cellObjects.begin(); it != cellObjects.end();)
	{
		const T* object = *it;
		const flat::AABB2& aabb = object->getAABB();
		Cell& newCell = updateObjectCell(cell, object, aabb);
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

template<class T, int depth>
inline QuadTreeCell<T>& QuadTree<T, depth>::updateObjectCell(Cell& cell, const T* object, const flat::AABB2& aabb)
{
	if (!cell.contains(aabb))
	{
		return findParentCellForAABB(cell, aabb);
	}
	else
	{
		return findChildCellForAABB(cell, aabb);
	}
}

template<class T, int depth>
inline constexpr int QuadTree<T, depth>::cpow(int m, int n)
{
	if (n == 0)
	{
		return 1;
	}
	else if (n % 2 == 0)
	{
		int i = cpow(m, n / 2);
		return i * i;
	}
	else
	{
		return m * cpow(m, n - 1);
	}
}

template<class T, int depth>
inline constexpr int QuadTree<T, depth>::getNumCells()
{
	int numCells = 0;
	for (int i = 0; i < depth; ++i)
	{
		numCells += cpow(4, i);
	}
	return numCells;
}

} //geometry
} // flat

#endif // FLAT_GEOMETRY_QUADTREE_H


