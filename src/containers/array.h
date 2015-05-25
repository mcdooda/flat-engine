#ifndef FLAT_CONTAINERS_ARRAY_H
#define FLAT_CONTAINERS_ARRAY_H

#include <cstdlib>
#include <cstring>
#include "bitaccessor.h"
#include "../debug/assert.h"

namespace flat
{
namespace containers
{

template <class T>
class Array
{
	public:
		Array() :
			m_buffer(nullptr),
			m_size(0)
		{
			
		}
		
		Array(unsigned int size) :
			m_size(size)
		{
			m_buffer = malloc(size * sizeof(T));
		}
		
		Array(const Array<T>& array) :
			m_size(array.m_size)
		{
			m_buffer = malloc(m_size * sizeof(T));
			memcpy(m_buffer, array.m_buffer, m_size * sizeof(T));
		}
		
		void operator=(const Array<T>& array)
		{
			if (m_buffer)
				FLAT_FREE(m_buffer);
				
			m_size = array.m_size;
			
			size_t numBytes = m_size * sizeof(T);
			m_buffer = malloc(numBytes);
			memcpy(m_buffer, array.m_buffer, numBytes);
		}
		
		~Array()
		{
			FLAT_FREE(m_buffer);
		}
		
		void resize(unsigned int size)
		{
			m_size = size;
			m_buffer = realloc(m_buffer, size * sizeof(T));
		}
		
		bool isInRange(unsigned int index) const
		{
			return index < m_size;
		}
		
		unsigned int getSize() const
		{
			return m_size;
		}
		
		const T& operator[](unsigned int index) const
		{
			FLAT_ASSERT(isInRange(index));
			return m_buffer[index];
		}
		
		T& operator[](unsigned int index)
		{
			FLAT_ASSERT(isInRange(index));
			return m_buffer[index];
		}
		
		void memset(int c)
		{
			memset(m_buffer, c, m_size);
		}
		
		const T* getBuffer() const
		{
			return m_buffer;
		}
		
	private:
		T* m_buffer;
		const unsigned int m_size;
};

template <>
class Array<bool>
{	
	public:
		Array() :
			m_buffer(nullptr),
			m_size(0)
		{
			
		}
		
		Array(unsigned int size) :
			m_size(size)
		{
			m_buffer = static_cast<char*>(malloc(getBytesSize()));
		}
		
		Array(const Array<bool>& array) :
			m_size(array.m_size)
		{
			size_t bytesSize = getBytesSize();
			m_buffer = static_cast<char*>(malloc(bytesSize));
			memcpy(m_buffer, array.m_buffer, bytesSize);
		}
		
		void operator=(const Array<bool>& array)
		{
			if (m_buffer)
				free(m_buffer);
				
			m_size = array.m_size;
				
			size_t numBytes = getBytesSize();
			m_buffer = static_cast<char*>(malloc(numBytes));
			memcpy(m_buffer, array.m_buffer, numBytes);
		}
		
		~Array()
		{
			free(m_buffer);
			m_buffer = nullptr;
		}
		
		void resize(unsigned int size)
		{
			m_size = size;
			m_buffer = static_cast<char*>(realloc(m_buffer, getBytesSize()));
		}
		
		bool isInRange(unsigned int index) const
		{
			return index < m_size;
		}
		
		bool operator[](unsigned int index) const
		{
			FLAT_ASSERT(isInRange(index));
			unsigned int byteIndex = index / 8;
			unsigned char bitIndex = index % 8;
			char bit = (m_buffer[byteIndex] >> bitIndex) & 0x01;
			return bit;
		}
		
		BitAccessor operator[](unsigned int index)
		{
			FLAT_ASSERT(isInRange(index));
			unsigned int byteIndex = index / 8;
			unsigned int bitIndex = index % 8;
			return BitAccessor(m_buffer, byteIndex, bitIndex);
		}
		
		void memset(int c)
		{
			::memset(m_buffer, c, getBytesSize());
		}
		
	private:
		size_t getBytesSize() const
		{
			return ceilf(static_cast<float>(m_size) / 8.f);
		}
		
	private:
		char* m_buffer;
		unsigned int m_size;
};

} // containers
} // flat

#endif // FLAT_CONTAINERS_FIXEDSIZEARRAY_H



