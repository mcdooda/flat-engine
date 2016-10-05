#include <array>
#include "../debug/assert.h"

namespace flat
{
namespace containers
{

template <class T>
union alignas(alignof(T)) PoolEntry
{
	uint8_t objectData[sizeof(T)];
	PoolEntry<T>* next;
};

template <class T, unsigned int Size>
class Pool : public std::array<PoolEntry<T>, Size>
{
public:
	Pool()
		FLAT_DEBUG_ONLY(: m_numAllocatedObjects(0))
	{
		for (unsigned int i = 0; i < Size - 1; ++i)
		{
			setNext((*this)[i], (*this)[i + 1]);
		}
		last().next = nullptr;
		m_head = &first();
	}

	~Pool()
	{
		FLAT_ASSERT(m_numAllocatedObjects == 0);
	}

	template <typename... ConstructorArgs>
	T* create(ConstructorArgs... constructorArgs)
	{
		FLAT_ASSERT(m_head != nullptr);
		T* object = reinterpret_cast<T*>(&m_head->objectData);
		m_head = m_head->next;
		new (object) T(constructorArgs...);
		FLAT_DEBUG_ONLY(++m_numAllocatedObjects);
		return object;
	}

	void destroy(T* object)
	{
		FLAT_ASSERT(object != nullptr);
		PoolEntry<T>& entry = reinterpret_cast<PoolEntry<T>&>(*object);
		FLAT_ASSERT(indexOf(entry) >= 0);
		object->~T();
		setNext(entry, *m_head);
		m_head = &entry;
		FLAT_DEBUG_ONLY(--m_numAllocatedObjects);
	}

private:
	void setNext(PoolEntry<T>& entry, PoolEntry<T>& next)
	{
		FLAT_ASSERT(indexOf(entry) >= 0);
		FLAT_ASSERT(indexOf(next) >= 0);
		entry.next = &next;
	}

	inline PoolEntry<T>& first()
	{
		return (*this)[0];
	}

	inline PoolEntry<T>& last()
	{
		return (*this)[Size - 1];
	}

	std::ptrdiff_t indexOf(const PoolEntry<T>& entry)
	{
		if (&first() <= &entry && &entry <= &last())
			return &entry - &first();

		return -1;
	}

private:
	PoolEntry<T>* m_head;
	FLAT_DEBUG_ONLY(int m_numAllocatedObjects);
};

} // containers
} // flat

