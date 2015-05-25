#ifndef FLAT_CONTAINERS_FIXEDSIZEARRAY_H
#define FLAT_CONTAINERS_FIXEDSIZEARRAY_H

#include <cstring>
#include "bitaccessor.h"
#include "../debug/assert.h"

namespace flat
{
namespace containers
{

template <class T, unsigned int SIZE>
class FixedSizeArray
{
	public:
		FixedSizeArray() {}
		~FixedSizeArray() {}
		
		bool isInRange(unsigned int index) const
		{
			return index < SIZE;
		}
		
		unsigned int getSize() const
		{
			return SIZE;
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
			::memset(m_buffer, c, SIZE);
		}
		
		const T* getBuffer() const
		{
			return m_buffer;
		}
		
	private:
		T m_buffer[SIZE];
};

constexpr int getNumBytes(int numBits)
{
	return static_cast<float>(numBits) / 8.f == static_cast<float>(numBits / 8) ? numBits / 8 : numBits / 8 + 1;
}

template <int SIZE>
class FixedSizeArray<bool, SIZE>
{
	public:
		FixedSizeArray() :
			m_buffer(nullptr)
		{
			m_buffer = static_cast<char*>(malloc(getNumBytes(SIZE)));
		}
		
		FixedSizeArray(const FixedSizeArray<bool, SIZE>& array)
		{
			size_t bytesSize = getNumBytes(SIZE);
			m_buffer = static_cast<char*>(malloc(bytesSize));
			memcpy(m_buffer, array.m_buffer, bytesSize);
		}
		
		void operator=(const FixedSizeArray<bool, SIZE>& array)
		{
			if (m_buffer)
				FLAT_FREE(m_buffer);
				
			size_t numBytes = getNumBytes(SIZE);
			m_buffer = static_cast<char*>(malloc(numBytes));
			memcpy(m_buffer, array.m_buffer, numBytes);
		}
		
		~FixedSizeArray()
		{
			FLAT_FREE(m_buffer);
		}
		
		bool isInRange(unsigned int index) const
		{
			return index < SIZE;
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
			::memset(m_buffer, c, getNumBytes(SIZE));
		}
		
	private:
		char m_buffer[getNumBytes(SIZE)];
};

} // containers
} // flat

#endif // FLAT_CONTAINERS_FIXEDSIZEARRAY_H



