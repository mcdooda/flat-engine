#ifndef FLAT_GEOMETRY_QUADTREE_H
#define FLAT_GEOMETRY_QUADTREE_H

#include <vector>
#include "../misc/aabb2.h"

namespace flat
{
namespace geometry
{

template <class T>
inline void getDefaultAABB(T* object, AABB2& aabb)
{
	aabb = object->getAABB();
}

template <class T>
class QuadTreeCell
{
	public:
		QuadTreeCell() = default;
		~QuadTreeCell() = default;

		inline void setAABB(const AABB2& aabb) { m_aabb = aabb; }
#ifdef FLAT_DEBUG
		inline const AABB2& getAABB() const { return m_aabb; }
#endif

		inline const std::vector<T*>& getObjects() const { return m_objects; }
		inline std::vector<T*>& getObjects() { return m_objects; }
		void addObject(T* object);
		void removeObject(T* object);
		void clear();

		inline bool isInside(const Vector2& point) const { return m_aabb.isInside(point); }
		inline bool contains(const AABB2& aabb) const { return m_aabb.contains(aabb); }
		inline bool overlap(const AABB2& aabb) const { return AABB2::overlap(m_aabb, aabb); }

	private:
		AABB2 m_aabb;
		std::vector<T*> m_objects;
};

template <class T>
inline void QuadTreeCell<T>::addObject(T* object)
{
	FLAT_ASSERT(std::find(m_objects.begin(), m_objects.end(), object) == m_objects.end());
	m_objects.push_back(object);
}

template <class T>
inline void QuadTreeCell<T>::removeObject(T* object)
{
	auto it = std::find(m_objects.begin(), m_objects.end(), object);
	FLAT_ASSERT(it != m_objects.end());
	m_objects.erase(it);
}

template <class T>
inline void QuadTreeCell<T>::clear()
{
	m_objects.clear();
}

template <class T, int depth, void (*GetAABB)(T*, AABB2&) = getDefaultAABB<T>>
class QuadTree
{
	public:
		using Cell = QuadTreeCell<T>;

	private:
		enum ChildPosition
		{
			BOTTOM_LEFT  = 1,
			BOTTOM_RIGHT = 2,
			TOP_LEFT     = 3,
			TOP_RIGHT    = 4
		};

	public:
		QuadTree(const AABB2& aabb);
		~QuadTree() = default;

		int addObject(T* object);
		int updateObject(T* object, int previousCellIndex);
		void removeObject(T* object);
		void removeObject(T* object, int cellIndex);
		void updateAllObjects();
		void updateAllObjects(std::unordered_map<T*, int>& objectCellIndices);
		void clear();
		void getObjects(const AABB2& aabb, std::vector<T*>& objects) const;
		void getObjects(const Vector2& point, std::vector<T*>& objects) const;
		template <typename Func>
		void eachObject(const AABB2& aabb, Func func) const;
		template <typename Func>
		void eachObject(const Vector2& point, Func func) const;

#ifdef FLAT_DEBUG
		const Cell& getCell(int index) const { return m_cells[index]; }
#endif

	private:
		void initAABBs(Cell& cell, const AABB2& aabb);
		int  getCellIndex(const Cell& cell) const;
		bool isRoot(const Cell& cell) const;
		bool isLeaf(const Cell& cell) const;
		Cell& getRootCell();
		const Cell& getRootCell() const;
		Cell& getChild(const Cell& cell, ChildPosition childPosition);
		const Cell& getChild(const Cell& cell, ChildPosition childPosition) const;
		Cell& getParentCell(Cell& cell);
		const Cell& getParentCell(const Cell& cell) const;
		Cell& findChildCellForAABB(Cell& cell, const AABB2& aabb);

		void getObjectsInCell(const Cell& cell, const AABB2& aabb, std::vector<T*>& objects) const;
		void getObjectsInCell(const Cell& cell, const Vector2& point, std::vector<T*>& objects) const;

		template <typename Func>
		void eachObjectInCell(const Cell& cell, const AABB2& aabb, Func func) const;

		template <typename Func>
		void eachObjectInCell(const Cell& cell, const Vector2& point, Func func) const;

		void updateCellObjects(Cell& cell);
		Cell& updateObjectCell(Cell& cell, const AABB2& aabb);

		static constexpr int cpow(int m, int n);
		static constexpr int getNumCells();

	private:
		static constexpr int NUM_CELLS = getNumCells();
		std::array<Cell, NUM_CELLS> m_cells;
};

template <class T, int depth, void (*GetAABB)(T*, AABB2&)>
inline QuadTree<T, depth, GetAABB>::QuadTree(const AABB2& aabb)
{
	initAABBs(getRootCell(), aabb);
}

template <class T, int depth, void (*GetAABB)(T*, AABB2&)>
inline int QuadTree<T, depth, GetAABB>::addObject(T* object)
{
	AABB2 aabb;
	GetAABB(object, aabb);
	FLAT_ASSERT_MSG(getRootCell().contains(aabb), "AABB is outside of the quad tree's root cell");
	Cell& cell = findChildCellForAABB(getRootCell(), aabb);
	cell.addObject(object);
	return getCellIndex(cell);
}

template <class T, int depth, void (*GetAABB)(T*, AABB2&)>
inline int QuadTree<T, depth, GetAABB>::updateObject(T* object, int previousCellIndex)
{
	Cell& previousCell = m_cells[previousCellIndex];
	AABB2 aabb;
	GetAABB(object, aabb);
	Cell& newCell = updateObjectCell(previousCell, aabb);
	if (&newCell != &previousCell)
	{
		previousCell.removeObject(object);
		newCell.addObject(object);
	}
	return getCellIndex(newCell);
}

template <class T, int depth, void (*GetAABB)(T*, AABB2&)>
inline void QuadTree<T, depth, GetAABB>::removeObject(T* object)
{
	AABB2 aabb;
	GetAABB(object, aabb);
	Cell& cell = findChildCellForAABB(getRootCell(), aabb);
	cell.removeObject(object);
}

template <class T, int depth, void (*GetAABB)(T*, AABB2&)>
inline void QuadTree<T, depth, GetAABB>::removeObject(T* object, int cellIndex)
{
	m_cells[cellIndex].removeObject(object);
}

template <class T, int depth, void (*GetAABB)(T*, AABB2&)>
inline void QuadTree<T, depth, GetAABB>::updateAllObjects()
{
	for (int i = 0; i < NUM_CELLS; ++i)
	{
		Cell& cell = m_cells[i];
		updateCellObjects(cell);
	}
}

template <class T, int depth, void (*GetAABB)(T*, AABB2&)>
inline void QuadTree<T, depth, GetAABB>::updateAllObjects(std::unordered_map<T*, int>& objectCellIndices)
{
	updateAllObjects();
	for (int i = 0; i < NUM_CELLS; ++i)
	{
		Cell& cell = m_cells[i];
		for (T* object : cell.getObjects())
		{
			objectCellIndices[object] = i;
		}
	}
}

template <class T, int depth, void (*GetAABB)(T*, AABB2&)>
inline void QuadTree<T, depth, GetAABB>::clear()
{
	for (Cell<T>& cell : m_cells)
	{
		cell.clear();
	}
}

template <class T, int depth, void (*GetAABB)(T*, AABB2&)>
inline void QuadTree<T, depth, GetAABB>::getObjects(const AABB2& aabb, std::vector<T*>& objects) const
{
	FLAT_ASSERT(aabb.isValid());
	getObjectsInCell(getRootCell(), aabb, objects);
}

template <class T, int depth, void (*GetAABB)(T*, AABB2&)>
inline void QuadTree<T, depth, GetAABB>::getObjects(const Vector2& point, std::vector<T*>& objects) const
{
	getObjectsInCell(getRootCell(), point, objects);
}

template <class T, int depth, void(*GetAABB)(T*, AABB2&)>
template <typename Func>
inline void QuadTree<T, depth, GetAABB>::eachObject(const AABB2& aabb, Func func) const
{
	FLAT_ASSERT(aabb.isValid());
	eachObjectInCell(getRootCell(), aabb, func);
}

template <class T, int depth, void(*GetAABB)(T*, AABB2&)>
template <typename Func>
inline void QuadTree<T, depth, GetAABB>::eachObject(const Vector2& point, Func func) const
{
	eachObjectInCell(getRootCell(), point, func);
}

template <class T, int depth, void (*GetAABB)(T*, AABB2&)>
inline void QuadTree<T, depth, GetAABB>::initAABBs(Cell& cell, const AABB2& aabb)
{
	cell.setAABB(aabb);
	if (!isLeaf(cell))
	{
		Cell& bottomLeftCell = getChild(cell, ChildPosition::BOTTOM_LEFT);
		initAABBs(bottomLeftCell, AABB2(aabb.min, aabb.getCenter()));

		Cell& bottomRightCell = getChild(cell, ChildPosition::BOTTOM_RIGHT);
		initAABBs(bottomRightCell, AABB2(Vector2(aabb.getCenter().x, aabb.min.y), Vector2(aabb.max.x, aabb.getCenter().y)));

		Cell& topLeftCell = getChild(cell, ChildPosition::TOP_LEFT);
		initAABBs(topLeftCell, AABB2(Vector2(aabb.min.x, aabb.getCenter().y), Vector2(aabb.getCenter().x, aabb.max.y)));

		Cell& topRightCell = getChild(cell, ChildPosition::TOP_RIGHT);
		initAABBs(topRightCell, AABB2(aabb.getCenter(), aabb.max));
	}
}

template <class T, int depth, void (*GetAABB)(T*, AABB2&)>
inline int QuadTree<T, depth, GetAABB>::getCellIndex(const Cell& cell) const
{
	return static_cast<int>(&cell - &m_cells[0]);
}

template <class T, int depth, void (*GetAABB)(T*, AABB2&)>
inline bool QuadTree<T, depth, GetAABB>::isRoot(const Cell& cell) const
{
	return &m_cells[0] == &cell;
}

template <class T, int depth, void (*GetAABB)(T*, AABB2&)>
inline bool QuadTree<T, depth, GetAABB>::isLeaf(const Cell& cell) const
{
	int index = getCellIndex(cell);
	return index * 4 + 1 >= NUM_CELLS;
}

template <class T, int depth, void (*GetAABB)(T*, AABB2&)>
inline typename QuadTree<T, depth, GetAABB>::Cell& QuadTree<T, depth, GetAABB>::getRootCell()
{
	return m_cells[0];
}

template <class T, int depth, void (*GetAABB)(T*, AABB2&)>
inline const typename QuadTree<T, depth, GetAABB>::Cell& QuadTree<T, depth, GetAABB>::getRootCell() const
{
	return m_cells[0];
}

template <class T, int depth, void (*GetAABB)(T*, AABB2&)>
inline typename QuadTree<T, depth, GetAABB>::Cell& QuadTree<T, depth, GetAABB>::getChild(const Cell& cell, ChildPosition childPosition)
{
	int index = getCellIndex(cell);
	return m_cells[index * 4 + childPosition];
}

template <class T, int depth, void (*GetAABB)(T*, AABB2&)>
inline const typename QuadTree<T, depth, GetAABB>::Cell& QuadTree<T, depth, GetAABB>::getChild(const Cell& cell, ChildPosition childPosition) const
{
	int index = getCellIndex(cell);
	return m_cells[index * 4 + childPosition];
}

template <class T, int depth, void (*GetAABB)(T*, AABB2&)>
inline typename QuadTree<T, depth, GetAABB>::Cell& QuadTree<T, depth, GetAABB>::getParentCell(Cell& cell)
{
	int index = getCellIndex(cell);
	return m_cells[(index - 1) / 4];
}

template <class T, int depth, void (*GetAABB)(T*, AABB2&)>
inline const typename QuadTree<T, depth, GetAABB>::Cell& QuadTree<T, depth, GetAABB>::getParentCell(const Cell& cell) const
{
	int index = getCellIndex(cell);
	return m_cells[(index - 1) / 4];
}

template <class T, int depth, void (*GetAABB)(T*, AABB2&)>
inline typename QuadTree<T, depth, GetAABB>::Cell& QuadTree<T, depth, GetAABB>::findChildCellForAABB(Cell& cell, const AABB2& aabb)
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

template <class T, int depth, void (*GetAABB)(T*, AABB2&)>
inline void QuadTree<T, depth, GetAABB>::getObjectsInCell(const Cell& cell, const AABB2& aabb, std::vector<T*>& objects) const
{
	if (cell.overlap(aabb))
	{
		const std::vector<T*>& cellObjects = cell.getObjects();
		for (T* object : cellObjects)
		{
			AABB2 objectAABB;
			GetAABB(object, objectAABB);
			if (AABB2::overlap(aabb, objectAABB))
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

template <class T, int depth, void (*GetAABB)(T*, AABB2&)>
inline void QuadTree<T, depth, GetAABB>::getObjectsInCell(const Cell& cell, const Vector2& point, std::vector<T*>& objects) const
{
	if (cell.isInside(point))
	{
		const std::vector<T*>& cellObjects = cell.getObjects();
		for (T* object : cellObjects)
		{
			AABB2 objectAABB;
			GetAABB(object, objectAABB);
			if (objectAABB.isInside(point))
			{
				objects.push_back(object);
			}
		}

		if (!isLeaf(cell))
		{
			const Cell& bottomLeftCell = getChild(cell, ChildPosition::BOTTOM_LEFT);
			getObjectsInCell(bottomLeftCell, point, objects);

			const Cell& bottomRightCell = getChild(cell, ChildPosition::BOTTOM_RIGHT);
			getObjectsInCell(bottomRightCell, point, objects);

			const Cell& topLeftCell = getChild(cell, ChildPosition::TOP_LEFT);
			getObjectsInCell(topLeftCell, point, objects);

			const Cell& topRightCell = getChild(cell, ChildPosition::TOP_RIGHT);
			getObjectsInCell(topRightCell, point, objects);
		}
	}
}

template <class T, int depth, void(*GetAABB)(T*, AABB2&)>
template <typename Func>
inline void QuadTree<T, depth, GetAABB>::eachObjectInCell(const Cell& cell, const AABB2& aabb, Func func) const
{
	if (cell.overlap(aabb))
	{
		const std::vector<T*>& cellObjects = cell.getObjects();
		for (T* object : cellObjects)
		{
			AABB2 objectAABB;
			GetAABB(object, objectAABB);
			if (AABB2::overlap(aabb, objectAABB))
			{
				func(object);
			}
		}

		if (!isLeaf(cell))
		{
			const Cell& bottomLeftCell = getChild(cell, ChildPosition::BOTTOM_LEFT);
			eachObjectInCell(bottomLeftCell, aabb, func);

			const Cell& bottomRightCell = getChild(cell, ChildPosition::BOTTOM_RIGHT);
			eachObjectInCell(bottomRightCell, aabb, func);

			const Cell& topLeftCell = getChild(cell, ChildPosition::TOP_LEFT);
			eachObjectInCell(topLeftCell, aabb, func);

			const Cell& topRightCell = getChild(cell, ChildPosition::TOP_RIGHT);
			eachObjectInCell(topRightCell, aabb, func);
		}
	}
}

template <class T, int depth, void(*GetAABB)(T*, AABB2&)>
template <typename Func>
inline void QuadTree<T, depth, GetAABB>::eachObjectInCell(const Cell& cell, const Vector2& point, Func func) const
{
	if (cell.isInside(point))
	{
		const std::vector<T*>& cellObjects = cell.getObjects();
		for (T* object : cellObjects)
		{
			AABB2 objectAABB;
			GetAABB(object, objectAABB);
			if (objectAABB.isInside(point))
			{
				func(object);
			}
		}

		if (!isLeaf(cell))
		{
			const Cell& bottomLeftCell = getChild(cell, ChildPosition::BOTTOM_LEFT);
			getObjectsInCell(bottomLeftCell, point, func);

			const Cell& bottomRightCell = getChild(cell, ChildPosition::BOTTOM_RIGHT);
			getObjectsInCell(bottomRightCell, point, func);

			const Cell& topLeftCell = getChild(cell, ChildPosition::TOP_LEFT);
			getObjectsInCell(topLeftCell, point, func);

			const Cell& topRightCell = getChild(cell, ChildPosition::TOP_RIGHT);
			getObjectsInCell(topRightCell, point, func);
		}
	}
}

template <class T, int depth, void (*GetAABB)(T*, AABB2&)>
inline void QuadTree<T, depth, GetAABB>::updateCellObjects(Cell& cell)
{
	std::vector<T*>& cellObjects = cell.getObjects();
	for (std::vector<T*>::iterator it = cellObjects.begin(); it != cellObjects.end();)
	{
		T* object = *it;
		AABB2 aabb;
		GetAABB(object, aabb);
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

template <class T, int depth, void (*GetAABB)(T*, AABB2&)>
inline typename QuadTree<T, depth, GetAABB>::Cell& QuadTree<T, depth, GetAABB>::updateObjectCell(Cell& cell, const AABB2& aabb)
{
	if (cell.contains(aabb))
	{
		return findChildCellForAABB(cell, aabb);
	}
#ifdef FLAT_DEBUG
	else if (isRoot(cell))
	{
		FLAT_ASSERT_MSG(false, "AABB is outside of the quad tree's root cell");
		return cell;
	}
#endif
	else
	{
		Cell& parentCell = getParentCell(cell);
		return updateObjectCell(parentCell, aabb);
	}
}

template <class T, int depth, void (*GetAABB)(T*, AABB2&)>
inline constexpr int QuadTree<T, depth, GetAABB>::cpow(int m, int n)
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

template <class T, int depth, void (*GetAABB)(T*, AABB2&)>
inline constexpr int QuadTree<T, depth, GetAABB>::getNumCells()
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


