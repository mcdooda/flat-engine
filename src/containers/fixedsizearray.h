#ifndef FLAT_CONTAINERS_FIXEDSIZEARRAY_H
#define FLAT_CONTAINERS_FIXEDSIZEARRAY_H

#include <cstring>
#include "bitaccessor.h"
#include "../debug/assert.h"
#include "../memory/memory.h"

namespace flat
{
namespace containers
{

template <class T, unsigned int Size>
class FixedSizeArray
{
	public:
		FixedSizeArray() {}
		~FixedSizeArray() {}
		
		bool isInRange(unsigned int index) const
		{
			return index < Size;
		}

		int indexOf(const T& object) const
		{
			std::ptrdiff_t index = &object - &m_buffer[0];
			if (index >= 0 && index < Size)
				return static_cast<int>(index);

			return -1;
		}
		
		unsigned int getSize() const
		{
			return Size;
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
			::memset(m_buffer, c, Size);
		}
		
		const T* getBuffer() const
		{
			return m_buffer;
		}
		
	protected:
		T m_buffer[Size];
};

constexpr int getNumBytes(int numBits)
{
	return static_cast<float>(numBits) / 8.f == static_cast<float>(numBits / 8) ? numBits / 8 : numBits / 8 + 1;
}

template <int Size>
class FixedSizeArray<bool, Size>
{
	public:
		FixedSizeArray() {}
		~FixedSizeArray() {}
		
		bool isInRange(unsigned int index) const
		{
			return index < Size;
		}
		
		bool operator[](unsigned int index) const
		{
			FLAT_ASSERT(isInRange(index));
			unsigned int byteIndex = index / 8;
			unsigned char bitIndex = index % 8;
			char bit = (m_buffer[byteIndex] >> bitIndex) & 0x01;
			return bit == 1;
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
			::memset(m_buffer, c, getNumBytes(Size));
		}
		
	private:
		char m_buffer[getNumBytes(Size)];
};

} // containers
} // flat

#endif // FLAT_CONTAINERS_FIXEDSIZEARRAY_H



