#ifndef FLAT_CONTAINERS_DYNAMICPOOL_H
#define FLAT_CONTAINERS_DYNAMICPOOL_H

#include <cstdlib>
#include "../debug/assert.h"

namespace flat
{
namespace containers
{

class DynamicPool
{
public:
	DynamicPool() :
		m_buffer(nullptr),
		m_head(nullptr)
#ifdef FLAT_DEBUG
		, m_numAllocatedObjects(0)
#endif
	{
		
	}

	~DynamicPool()
	{
		FLAT_ASSERT(m_numAllocatedObjects == 0);
		free(m_buffer);
	}

	template <class T, unsigned int Size>
	void init()
	{
		init(sizeof(T), Size);
	}

	void init(size_t objectSize, size_t size)
	{
		FLAT_ASSERT(m_buffer == nullptr);
		FLAT_ASSERT(objectSize >= sizeof(void*));
		m_size = size;
		m_objectSize = objectSize;
		m_buffer = malloc(size * objectSize);

		for (unsigned int i = 0; i < size - 1; ++i)
		{
			void* entry = reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(m_buffer) + i * objectSize);
			void* next = reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(m_buffer) + (i + 1) * objectSize);
			setNext(entry, next);
		}
		*reinterpret_cast<void**>(last()) = nullptr;
		m_head = first();
	}

	template <class T, typename... ConstructorArgs>
	T* create(ConstructorArgs... constructorArgs)
	{
		FLAT_ASSERT(m_numAllocatedObjects < m_size);
		FLAT_ASSERT(sizeof(T) == m_objectSize);
		FLAT_ASSERT(m_head != nullptr);
		T* object = reinterpret_cast<T*>(m_head);
		m_head = *reinterpret_cast<void**>(m_head);
		new (object) T(constructorArgs...);
		FLAT_DEBUG_ONLY(++m_numAllocatedObjects;)
		return object;
	}

	template <class T>
	void destroy(T* object)
	{
		FLAT_ASSERT(sizeof(T) == m_objectSize);
		FLAT_ASSERT(object != nullptr);
		void* entry = reinterpret_cast<void*>(object);
		FLAT_ASSERT(indexOf(entry) >= 0);
		object->~T();
		setNext(entry, m_head);
		m_head = entry;
		FLAT_DEBUG_ONLY(--m_numAllocatedObjects;)
	}

private:
	void setNext(void* entry, void* next)
	{
		FLAT_ASSERT(indexOf(entry) >= 0);
		FLAT_ASSERT(indexOf(next) >= 0);
		*reinterpret_cast<void**>(entry) = next;
	}

	inline void* first()
	{
		return m_buffer;
	}

	inline void* last()
	{
		return reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(m_buffer) + (m_size - 1) * m_objectSize);
	}

	std::ptrdiff_t indexOf(void* entry)
	{
		if (first() <= entry && entry <= last())
			return reinterpret_cast<unsigned char*>(entry) - reinterpret_cast<unsigned char*>(first());

		return -1;
	}

private:
	void* m_buffer;
	void* m_head;
	size_t m_size;
	size_t m_objectSize;
	FLAT_DEBUG_ONLY(int m_numAllocatedObjects;)
};

} // containers
} // flat

#endif // FLAT_CONTAINERS_DYNAMICPOOL_H


