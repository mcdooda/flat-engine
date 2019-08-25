#ifndef FLAT_CONTAINERS_HYBRIDARRAY_H
#define FLAT_CONTAINERS_HYBRIDARRAY_H

#include <vector>
#include <memory>
#include <cstring>

namespace flat
{
namespace containers
{

template <class T, int StaticSize>
class HybridArray
{
	static constexpr unsigned int BufferSize = StaticSize * sizeof(T);
	public:
		class Iterator
		{
			public:
				Iterator(const HybridArray<T, StaticSize>* array, T* pointer) :
					m_array(array),
					m_pointer(pointer)
				{}

				Iterator& operator++()
				{
					++m_pointer;
					if (m_pointer - reinterpret_cast<const T*>(&m_array->m_buffer[0]) == m_array->m_arrayCount)
					{
						if (std::vector<T>* vector = m_array->m_vector.get())
						{
							m_pointer = &*vector->begin();
						}
					}
					return *this;
				}

				T& operator*()
				{
					return *m_pointer;
				}

				T* operator->()
				{
					return m_pointer;
				}

				bool operator==(const Iterator& other)
				{
					FLAT_ASSERT(m_array == other.m_array);
					return m_pointer == other.m_pointer;
				}

				bool operator!=(const Iterator& other)
				{
					return !(*this == other);
				}

			private:
				const HybridArray<T, StaticSize>* m_array;
				T* m_pointer;
		};

	public:
		HybridArray() :
			m_arrayCount(0)
		{
			FLAT_INIT_MEMORY(&m_buffer[0], BufferSize);
		}

		~HybridArray()
		{
			for (unsigned int i = 0; i < m_arrayCount; ++i)
			{
				T* object = reinterpret_cast<T*>(&m_buffer[0]) + i;
				object->~T();
			}

			FLAT_WIPE_MEMORY(&m_buffer[0], BufferSize);
		}
		
		bool isInRange(unsigned int index) const
		{
			if (const std::vector<T>* vector = m_vector.get())
			{
				FLAT_ASSERT(m_arrayCount == StaticSize);
				return index < m_arrayCount + vector->size();
			}
			return index < m_arrayCount;
		}

		int indexOf(const T& object) const
		{
			int index = &object - reinterpret_cast<const T*>(&m_buffer[0]);
			if (0 <= index && index < m_arrayCount)
			{
				return index;
			}
			else if (const std::vector<T>* vector = m_vector.get())
			{
				FLAT_ASSERT(m_arrayCount == StaticSize);
				if (vector->begin() <= &object && &object < vector->end())
				{
					return (&object - vector->begin()) + StaticSize;
				}
			}
			return -1;
		}
		
		unsigned int getSize() const
		{
			if (const std::vector<T>* vector = m_vector.get())
			{
				FLAT_ASSERT(m_arrayCount == StaticSize);
				return StaticSize + static_cast<unsigned int>(vector->size());
			}
			return m_arrayCount;
		}
		
		const T& operator[](unsigned int index) const
		{
			FLAT_ASSERT(isInRange(index));
			if (index < m_arrayCount)
			{
				return *(reinterpret_cast<const T*>(&m_buffer[0]) + index);
			}
			FLAT_ASSERT(m_arrayCount == StaticSize);
			FLAT_ASSERT(m_vector != nullptr);
			return (*m_vector.get())[index - StaticSize];
		}
		
		T& operator[](unsigned int index)
		{
			FLAT_ASSERT(isInRange(index));
			if (index < m_arrayCount)
			{
				return *(reinterpret_cast<T*>(&m_buffer[0]) + index);
			}
			FLAT_ASSERT(m_arrayCount == StaticSize);
			FLAT_ASSERT(m_vector != nullptr);
			return (*m_vector.get())[index - StaticSize];
		}
		
		void memset(int c)
		{
			std::memset(m_buffer, c, m_arrayCount);
			if (const std::vector<T>* vector = m_vector.get())
			{
				FLAT_ASSERT(m_arrayCount == StaticSize);
				std::memset(vector->begin(), c, vector->size());
			}
		}

		template <typename... ConstructorArgs>
		T& add(ConstructorArgs... constructorArgs)
		{
			if (m_arrayCount < StaticSize)
			{
				T* object = reinterpret_cast<T*>(&m_buffer[0]) + m_arrayCount;
				new (object) T(constructorArgs...);
				++m_arrayCount;
				return *object;
			}

			FLAT_ASSERT(m_arrayCount == StaticSize);

			std::vector<T>* vector = m_vector.get();
			if (vector == nullptr)
			{
				m_vector = std::make_unique<std::vector<T>>();
				vector = m_vector.get();
				vector->reserve(StaticSize / 2);
			}
			return vector->emplace_back(constructorArgs...);
		}

		template <typename... ConstructorArgs>
		void push_back(ConstructorArgs... constructorArgs)
		{
			add(constructorArgs...);
		}

		Iterator begin()
		{
			return Iterator(this, reinterpret_cast<T*>(&m_buffer[0]));
		}

		Iterator end()
		{
			if (std::vector<T>* vector = m_vector.get())
			{
				FLAT_ASSERT(m_arrayCount == StaticSize);
				T* endPointer = &*vector->end();
				return Iterator(this, endPointer);
			}
			return Iterator(this, reinterpret_cast<T*>(&m_buffer[0]) + m_arrayCount);
		}
		
	private:
		std::uint8_t m_buffer[BufferSize];
		unsigned int m_arrayCount;
		std::unique_ptr<std::vector<T>> m_vector;
};

} // containers
} // flat

#endif // FLAT_CONTAINERS_HYBRIDARRAY_H



