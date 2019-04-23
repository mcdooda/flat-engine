#ifndef FLAT_GEOMETRY_QUADTREE_H
#define FLAT_GEOMETRY_QUADTREE_H

#ifdef FLAT_DEBUG
#include <set>
#endif

#include "misc/aabb2.h"
#include "debug/helpers.h"

namespace flat
{
namespace geometry
{

template <class T, int depth, void (*GetAABB)(T, AABB2&)>
class QuadTree;

template <class T>
inline void getDefaultAABB(T object, AABB2& aabb)
{
	aabb = object->getAABB();
}

enum CellAvailability : std::uint32_t
{
	AVAILABLE = 0xFFFFFFFF
};

enum CellIndex : std::uint32_t
{
	INVALID = 0xFFFFFFFF,
	INVALID_AVAILABLE = 0xFFFFFFFE
};

template <class T>
class QuadTreeCell
{
	template <class T, int depth, void (*GetAABB)(T, AABB2&)>
	friend class QuadTree;

	public:
		QuadTreeCell() : m_cellDataCount(0), m_cellDataIndex(CellIndex::INVALID) {}
		~QuadTreeCell() = default;

		inline void setAABB(const AABB2& aabb) { m_aabb = aabb; }
#ifdef FLAT_DEBUG
		inline const AABB2& getAABB() const { return m_aabb; }
#endif

		inline bool isInside(const Vector2& point) const { return m_aabb.isInside(point); }
		inline bool contains(const AABB2& aabb) const { return m_aabb.contains(aabb); }
		inline bool overlap(const AABB2& aabb) const { return AABB2::overlap(m_aabb, aabb); }

	private:
		AABB2 m_aabb;
		std::uint32_t m_cellDataIndex;
		int m_cellDataCount;
};

template <class T, int depth, void (*GetAABB)(T, AABB2&) = nullptr>
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

		int addObject(T object);
		int addObject(T object, const AABB2& objectAABB);
		int updateObject(T object, int previousCellIndex);
		int updateObject(T object, int previousCellIndex, const AABB2& objectAABB);
		void removeObject(T object);
		void removeObject(T object, const AABB2& objectAABB);
		void removeObject(T object, int cellIndex);
		void clear();

		template <class Container>
		void getObjects(const AABB2& aabb, Container& objects) const;

		template <class Container>
		void getObjects(const Vector2& point, Container& objects) const;

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

		template <class Container>
		void getObjectsInCell(const Cell& cell, const AABB2& aabb, Container& objects) const;

		template <class Container>
		void getObjectsInCell(const Cell& cell, const Vector2& point, Container& objects) const;

		template <typename Func>
		void eachObjectInCell(const Cell& cell, const AABB2& aabb, Func func) const;

		template <typename Func>
		void eachObjectInCell(const Cell& cell, const Vector2& point, Func func) const;

		Cell& updateObjectCell(Cell& cell, const AABB2& aabb);

		void addObjectInCell(Cell& cell, T object, const AABB2& objectAABB);
		void updateObjectInCell(Cell& cell, T object, const AABB2& objectAABB);
		void removeObjectInCell(Cell& cell, T object);

#ifdef FLAT_DEBUG
		void checkFreeCellDataListIntegrity();
#endif

		static constexpr int cpow(int m, int n);
		static constexpr int getNumCells();

	private:
		static constexpr int NUM_CELLS = getNumCells();

		std::array<Cell, NUM_CELLS> m_cells;

		union CellData
		{
			public:
				CellData(T object, const AABB2& objectAABB) :
					m_object(object),
					m_objectAABB(objectAABB)
				{
					FLAT_ASSERT(!isAvailable());
				}

				CellData(const CellData& other) :
					m_object(other.m_object),
					m_objectAABB(other.m_objectAABB)
				{
				}

				void operator=(const CellData& other)
				{
					FLAT_ASSERT(!isAvailable());
					m_object = other.m_object;
					m_objectAABB = other.m_objectAABB;
				}

				inline bool operator==(T object)
				{
					FLAT_ASSERT(!isAvailable());
					return m_object == object;
				}

				inline void setObject(T object, const AABB2& objectAABB)
				{
					m_object = object;
					m_objectAABB = objectAABB;
					FLAT_ASSERT(!isAvailable());
				}

				inline T getObject() const
				{
					FLAT_ASSERT(!isAvailable());
					return m_object;
				}

				inline const AABB2& getObjectAABB() const
				{
					FLAT_ASSERT(!isAvailable());
					FLAT_ASSERT(m_objectAABB.isValid());
					return m_objectAABB;
				}

				inline void updateAABB(const AABB2& objectAABB)
				{
					FLAT_ASSERT(!isAvailable());
					FLAT_ASSERT(m_objectAABB.isValid());
					FLAT_ASSERT(objectAABB.isValid());
					m_objectAABB = objectAABB;
				}

				inline void setAvailable(std::uint32_t nextFreeCellDataIndex)
				{
					m_nextFreeCellDataIndex = nextFreeCellDataIndex;
					m_availability = CellAvailability::AVAILABLE;
				}

				inline bool isAvailable() const
				{
					return m_availability == CellAvailability::AVAILABLE;
				}

				inline std::int32_t getNextFreeCellDataIndex() const
				{
					FLAT_ASSERT(isAvailable());
					return m_nextFreeCellDataIndex;
				}

			private:
				struct 
				{
					T m_object;
					AABB2 m_objectAABB;
				};
				struct
				{
					std::int32_t m_nextFreeCellDataIndex;
					CellAvailability m_availability;
				};
		};
		std::vector<CellData> m_cellData;
		std::int32_t m_firstFreeCellDataIndex;

#ifdef FLAT_DEBUG
		std::set<T> m_objects;
#endif
};

template <class T, int depth, void (*GetAABB)(T, AABB2&)>
inline QuadTree<T, depth, GetAABB>::QuadTree(const AABB2& aabb) :
	m_firstFreeCellDataIndex(CellIndex::INVALID)
{
	initAABBs(getRootCell(), aabb);
}

template <class T, int depth, void (*GetAABB)(T, AABB2&)>
inline int QuadTree<T, depth, GetAABB>::addObject(T object)
{
	FLAT_ASSERT(GetAABB != nullptr);
	AABB2 aabb;
	GetAABB(object, aabb);
	return addObject(object, aabb);
}

template <class T, int depth, void (*GetAABB)(T, AABB2&)>
inline int QuadTree<T, depth, GetAABB>::addObject(T object, const AABB2& objectAABB)
{
	FLAT_ASSERT(objectAABB.isValid());
	FLAT_ASSERT(m_objects.find(object) == m_objects.end());
	FLAT_DEBUG_ONLY(m_objects.insert(object);)
	FLAT_ASSERT_MSG(getRootCell().contains(objectAABB), "AABB is outside of the quad tree's root cell");
	Cell& cell = findChildCellForAABB(getRootCell(), objectAABB);
	addObjectInCell(cell, object, objectAABB);
	return getCellIndex(cell);
}

template <class T, int depth, void (*GetAABB)(T, AABB2&)>
inline int QuadTree<T, depth, GetAABB>::updateObject(T object, int previousCellIndex)
{
	FLAT_ASSERT(GetAABB != nullptr);
	AABB2 aabb;
	GetAABB(object, aabb);
	return updateObject(object, previousCellIndex, aabb);
}

template <class T, int depth, void (*GetAABB)(T, AABB2&)>
inline int QuadTree<T, depth, GetAABB>::updateObject(T object, int previousCellIndex, const AABB2& objectAABB)
{
	FLAT_ASSERT(objectAABB.isValid());
	FLAT_ASSERT(m_objects.find(object) != m_objects.end());
	Cell& previousCell = m_cells[previousCellIndex];
	Cell& newCell = updateObjectCell(previousCell, objectAABB);
	if (&newCell != &previousCell)
	{
		removeObjectInCell(previousCell, object);
		addObjectInCell(newCell, object, objectAABB);
	}
	else
	{
		updateObjectInCell(previousCell, object, objectAABB);
	}
	return getCellIndex(newCell);
}

template <class T, int depth, void (*GetAABB)(T, AABB2&)>
inline void QuadTree<T, depth, GetAABB>::removeObject(T object)
{
	AABB2 aabb;
	GetAABB(object, aabb);
	removeObject(object, aabb);
}

template <class T, int depth, void (*GetAABB)(T, AABB2&)>
inline void QuadTree<T, depth, GetAABB>::removeObject(T object, const AABB2& objectAABB)
{
	FLAT_ASSERT(objectAABB.isValid());
	FLAT_ASSERT(m_objects.find(object) != m_objects.end());
	FLAT_DEBUG_ONLY(m_objects.erase(object);)
	Cell& cell = findChildCellForAABB(getRootCell(), objectAABB);
	removeObjectInCell(cell, object);
}

template <class T, int depth, void (*GetAABB)(T, AABB2&)>
inline void QuadTree<T, depth, GetAABB>::removeObject(T object, int cellIndex)
{
	FLAT_ASSERT(m_objects.find(object) != m_objects.end());
	FLAT_DEBUG_ONLY(m_objects.erase(object);)
	removeObjectInCell(m_cells[cellIndex], object);
}

template <class T, int depth, void (*GetAABB)(T, AABB2&)>
inline void QuadTree<T, depth, GetAABB>::clear()
{
	FLAT_DEBUG_ONLY(m_objects.clear();)
	for (Cell<T>& cell : m_cells)
	{
		cell.m_cellDataIndex = CellIndex::INVALID;
		cell.m_cellDataCount = 0;
	}
	m_cellData.clear();
}

template <class T, int depth, void (*GetAABB)(T, AABB2&)>
template <class Container>
inline void QuadTree<T, depth, GetAABB>::getObjects(const AABB2& aabb, Container& objects) const
{
	FLAT_ASSERT(aabb.isValid());
	getObjectsInCell(getRootCell(), aabb, objects);
}

template <class T, int depth, void (*GetAABB)(T, AABB2&)>
template <class Container>
inline void QuadTree<T, depth, GetAABB>::getObjects(const Vector2& point, Container& objects) const
{
	getObjectsInCell(getRootCell(), point, objects);
}

template <class T, int depth, void (*GetAABB)(T, AABB2&)>
template <typename Func>
inline void QuadTree<T, depth, GetAABB>::eachObject(const AABB2& aabb, Func func) const
{
	FLAT_ASSERT(aabb.isValid());
	eachObjectInCell(getRootCell(), aabb, func);
}

template <class T, int depth, void (*GetAABB)(T, AABB2&)>
template <typename Func>
inline void QuadTree<T, depth, GetAABB>::eachObject(const Vector2& point, Func func) const
{
	eachObjectInCell(getRootCell(), point, func);
}

template <class T, int depth, void (*GetAABB)(T, AABB2&)>
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

template <class T, int depth, void (*GetAABB)(T, AABB2&)>
inline int QuadTree<T, depth, GetAABB>::getCellIndex(const Cell& cell) const
{
	return static_cast<int>(&cell - &m_cells[0]);
}

template <class T, int depth, void (*GetAABB)(T, AABB2&)>
inline bool QuadTree<T, depth, GetAABB>::isRoot(const Cell& cell) const
{
	return &m_cells[0] == &cell;
}

template <class T, int depth, void (*GetAABB)(T, AABB2&)>
inline bool QuadTree<T, depth, GetAABB>::isLeaf(const Cell& cell) const
{
	int index = getCellIndex(cell);
	return index * 4 + 1 >= NUM_CELLS;
}

template <class T, int depth, void (*GetAABB)(T, AABB2&)>
inline typename QuadTree<T, depth, GetAABB>::Cell& QuadTree<T, depth, GetAABB>::getRootCell()
{
	return m_cells[0];
}

template <class T, int depth, void (*GetAABB)(T, AABB2&)>
inline const typename QuadTree<T, depth, GetAABB>::Cell& QuadTree<T, depth, GetAABB>::getRootCell() const
{
	return m_cells[0];
}

template <class T, int depth, void (*GetAABB)(T, AABB2&)>
inline typename QuadTree<T, depth, GetAABB>::Cell& QuadTree<T, depth, GetAABB>::getChild(const Cell& cell, ChildPosition childPosition)
{
	int index = getCellIndex(cell);
	return m_cells[index * 4 + childPosition];
}

template <class T, int depth, void (*GetAABB)(T, AABB2&)>
inline const typename QuadTree<T, depth, GetAABB>::Cell& QuadTree<T, depth, GetAABB>::getChild(const Cell& cell, ChildPosition childPosition) const
{
	int index = getCellIndex(cell);
	return m_cells[index * 4 + childPosition];
}

template <class T, int depth, void (*GetAABB)(T, AABB2&)>
inline typename QuadTree<T, depth, GetAABB>::Cell& QuadTree<T, depth, GetAABB>::getParentCell(Cell& cell)
{
	int index = getCellIndex(cell);
	return m_cells[(index - 1) / 4];
}

template <class T, int depth, void (*GetAABB)(T, AABB2&)>
inline const typename QuadTree<T, depth, GetAABB>::Cell& QuadTree<T, depth, GetAABB>::getParentCell(const Cell& cell) const
{
	int index = getCellIndex(cell);
	return m_cells[(index - 1) / 4];
}

template <class T, int depth, void (*GetAABB)(T, AABB2&)>
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

template <class T, int depth, void (*GetAABB)(T, AABB2&)>
template <class Container>
inline void QuadTree<T, depth, GetAABB>::getObjectsInCell(const Cell& cell, const AABB2& aabb, Container& objects) const
{
	if (cell.overlap(aabb))
	{
		if (cell.m_cellDataCount > 0)
		{
			FLAT_ASSERT(cell.m_cellDataIndex != CellIndex::INVALID);
			std::vector<CellData>::const_iterator it = m_cellData.begin() + cell.m_cellDataIndex;
			std::vector<CellData>::const_iterator end = it + cell.m_cellDataCount;
			for (; it != end; ++it)
			{
				if (AABB2::overlap(aabb, it->getObjectAABB()))
				{
					objects.push_back(it->getObject());
				}
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

template <class T, int depth, void (*GetAABB)(T, AABB2&)>
template <class Container>
inline void QuadTree<T, depth, GetAABB>::getObjectsInCell(const Cell& cell, const Vector2& point, Container& objects) const
{
	if (cell.isInside(point))
	{
		if (cell.m_cellDataCount > 0)
		{
			FLAT_ASSERT(cell.m_cellDataIndex != CellIndex::INVALID);
			std::vector<CellData>::const_iterator it = m_cellData.begin() + cell.m_cellDataIndex;
			std::vector<CellData>::const_iterator end = it + cell.m_cellDataCount;
			for (; it != end; ++it)
			{
				if (it->getObjectAABB().isInside(point))
				{
					objects.push_back(it->getObject());
				}
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

template <class T, int depth, void (*GetAABB)(T, AABB2&)>
template <typename Func>
inline void QuadTree<T, depth, GetAABB>::eachObjectInCell(const Cell& cell, const AABB2& aabb, Func func) const
{
	if (cell.overlap(aabb))
	{
		if (cell.m_cellDataCount > 0)
		{
			FLAT_ASSERT(cell.m_cellDataIndex != CellIndex::INVALID);
			std::vector<CellData>::const_iterator it = m_cellData.begin() + cell.m_cellDataIndex;
			std::vector<CellData>::const_iterator end = it + cell.m_cellDataCount;
			for (; it != end; ++it)
			{
				if (AABB2::overlap(aabb, it->getObjectAABB()))
				{
					func(it->getObject());
				}
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

template <class T, int depth, void (*GetAABB)(T, AABB2&)>
template <typename Func>
inline void QuadTree<T, depth, GetAABB>::eachObjectInCell(const Cell& cell, const Vector2& point, Func func) const
{
	if (cell.isInside(point))
	{
		if (cell.m_cellDataCount > 0)
		{
			FLAT_ASSERT(cell.m_cellDataIndex != CellIndex::INVALID);
			std::vector<CellData>::const_iterator it = m_cellData.begin() + cell.m_cellDataIndex;
			std::vector<CellData>::const_iterator itEnd = it + cell.m_cellDataCount;
			for (; it != end; ++it)
			{
				if (it->getObjectAABB().isInside(point))
				{
					func(it->getObject());
				}
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

template <class T, int depth, void (*GetAABB)(T, AABB2&)>
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

template <class T, int depth, void (*GetAABB)(T, AABB2&)>
void QuadTree<T, depth, GetAABB>::addObjectInCell(Cell& cell, T object, const AABB2& objectAABB)
{
	FLAT_ASSERT(objectAABB.isValid());
	FLAT_ASSERT(m_firstFreeCellDataIndex == CellIndex::INVALID || m_cellData[m_firstFreeCellDataIndex].isAvailable());

	const std::int32_t cellDataCount = static_cast<std::int32_t>(m_cellData.size());

	if (cell.m_cellDataIndex == CellIndex::INVALID)
	{
		FLAT_ASSERT(cell.m_cellDataCount == 0);

		// find an available cell of allocate a new one
		if (m_firstFreeCellDataIndex != CellIndex::INVALID)
		{
			std::uint32_t nextFreeCellDataIndex = m_cellData[m_firstFreeCellDataIndex].getNextFreeCellDataIndex();
			FLAT_ASSERT(nextFreeCellDataIndex == CellIndex::INVALID || m_cellData[nextFreeCellDataIndex].isAvailable());

			// use the first free cell
			cell.m_cellDataIndex = m_firstFreeCellDataIndex;
			cell.m_cellDataCount = 1;

			m_cellData[cell.m_cellDataIndex].setObject(object, objectAABB);

			m_firstFreeCellDataIndex = nextFreeCellDataIndex;

			FLAT_ASSERT(m_firstFreeCellDataIndex == CellIndex::INVALID || m_cellData[m_firstFreeCellDataIndex].isAvailable());
		}
		else
		{
			// allocate
			cell.m_cellDataIndex = cellDataCount;
			cell.m_cellDataCount = 1;
			m_cellData.emplace_back(object, objectAABB);

			FLAT_ASSERT(m_firstFreeCellDataIndex == CellIndex::INVALID || m_cellData[m_firstFreeCellDataIndex].isAvailable());
		}
	}
	else
	{
		FLAT_ASSERT(cell.m_cellDataCount > 0);

		const std::int32_t newItemIndex = cell.m_cellDataIndex + cell.m_cellDataCount;
		FLAT_ASSERT(newItemIndex <= cellDataCount);
		if (newItemIndex == cellDataCount)
		{
			// simply put at the end
			++cell.m_cellDataCount;
			m_cellData.emplace_back(object, objectAABB);

			FLAT_ASSERT(m_firstFreeCellDataIndex == CellIndex::INVALID || m_cellData[m_firstFreeCellDataIndex].isAvailable());
		}
		else
		{
			// check if the cell data at newItemIndex is available or relocate
			if (m_cellData[newItemIndex].isAvailable()
				&& m_cellData[newItemIndex].getNextFreeCellDataIndex() == CellIndex::INVALID_AVAILABLE)
			{
				++cell.m_cellDataCount;
				m_cellData[newItemIndex].setObject(object, objectAABB);

				if (newItemIndex == m_firstFreeCellDataIndex)
				{
					m_firstFreeCellDataIndex = CellIndex::INVALID;
				}

				FLAT_ASSERT(m_firstFreeCellDataIndex == CellIndex::INVALID || m_cellData[m_firstFreeCellDataIndex].isAvailable());
			}
			else
			{
				FLAT_ASSERT(m_firstFreeCellDataIndex == CellIndex::INVALID || m_cellData[m_firstFreeCellDataIndex].isAvailable());

				// relocate at the end
				m_cellData.reserve(m_cellData.size() + cell.m_cellDataCount + 1);

				std::vector<CellData>::iterator begin = m_cellData.begin() + cell.m_cellDataIndex;
				std::vector<CellData>::iterator end = begin + cell.m_cellDataCount;
				for (std::vector<CellData>::iterator it = begin; it != end; ++it)
				{
					m_cellData.emplace_back(*it);
					it->setAvailable(CellIndex::INVALID_AVAILABLE);
				}

				FLAT_ASSERT(m_firstFreeCellDataIndex == CellIndex::INVALID || m_cellData[m_firstFreeCellDataIndex].isAvailable());
				begin->setAvailable(m_firstFreeCellDataIndex);
				m_firstFreeCellDataIndex = cell.m_cellDataIndex;

				++cell.m_cellDataCount;
				cell.m_cellDataIndex = cellDataCount;
				m_cellData.emplace_back(object, objectAABB);

				FLAT_ASSERT(m_firstFreeCellDataIndex == CellIndex::INVALID || m_cellData[m_firstFreeCellDataIndex].isAvailable());
			}
		}
	}
}

template <class T, int depth, void(*GetAABB)(T, AABB2&)>
void flat::geometry::QuadTree<T, depth, GetAABB>::updateObjectInCell(Cell& cell, T object, const AABB2& objectAABB)
{
	FLAT_ASSERT(cell.m_cellDataCount > 0);
	FLAT_ASSERT(cell.m_cellDataIndex != CellIndex::INVALID);

	std::vector<CellData>::iterator it = m_cellData.begin() + cell.m_cellDataIndex;
	std::vector<CellData>::iterator end = it + cell.m_cellDataCount;
	for (; it != end; ++it)
	{
		if (it->getObject() == object)
		{
			it->updateAABB(objectAABB);
			return;
		}
	}

	FLAT_ASSERT_MSG(false, "object not found in cell");
}

template <class T, int depth, void (*GetAABB)(T, AABB2&)>
void QuadTree<T, depth, GetAABB>::removeObjectInCell(Cell& cell, T object)
{
	FLAT_ASSERT(m_firstFreeCellDataIndex == CellIndex::INVALID || m_cellData[m_firstFreeCellDataIndex].isAvailable());

	FLAT_ASSERT(cell.m_cellDataCount > 0);
	FLAT_ASSERT(cell.m_cellDataIndex != CellIndex::INVALID);
	
	// find object in cell
	std::vector<CellData>::iterator it = m_cellData.begin() + cell.m_cellDataIndex;
	std::vector<CellData>::iterator end = it + cell.m_cellDataCount;
	it = std::find(it, end, object);
	FLAT_ASSERT(it != end);
	std::vector<CellData>::iterator last = end - 1;
	*it = *last;

	--cell.m_cellDataCount;

	if (cell.m_cellDataCount == 0)
	{
		FLAT_ASSERT(m_firstFreeCellDataIndex == CellIndex::INVALID || m_cellData[m_firstFreeCellDataIndex].isAvailable());
		last->setAvailable(m_firstFreeCellDataIndex);
		m_firstFreeCellDataIndex = cell.m_cellDataIndex;
		cell.m_cellDataIndex = CellIndex::INVALID;
	}
	else
	{
		last->setAvailable(CellIndex::INVALID_AVAILABLE);
	}

	FLAT_ASSERT(m_firstFreeCellDataIndex == CellIndex::INVALID || m_cellData[m_firstFreeCellDataIndex].isAvailable());
}

#ifdef FLAT_DEBUG
template <class T, int depth, void (*GetAABB)(T, AABB2&)>
void QuadTree<T, depth, GetAABB>::checkFreeCellDataListIntegrity()
{
	int cellIndex = m_firstFreeCellDataIndex;
	while (cellIndex != CellIndex::INVALID)
	{
		FLAT_ASSERT_MSG(m_cellData[cellIndex].isAvailable(), "Cell index %d is not available", cellIndex);
		cellIndex = m_cellData[cellIndex].getNextFreeCellDataIndex();
	}
}
#endif

template <class T, int depth, void (*GetAABB)(T, AABB2&)>
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

template <class T, int depth, void (*GetAABB)(T, AABB2&)>
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


