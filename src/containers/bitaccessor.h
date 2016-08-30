#ifndef FLAT_CONTAINERS_BITACCESSOR
#define FLAT_CONTAINERS_BITACCESSOR

#include "../debug/assert.h"

namespace flat
{
namespace containers
{

class BitAccessor
{
	public:
		BitAccessor(char* buffer, unsigned int byteIndex, unsigned char bitIndex) :
			m_buffer(buffer),
			m_byteIndex(byteIndex),
			m_bitIndex(bitIndex)
		{
			FLAT_ASSERT(bitIndex < 8);
		}
		
		void operator=(bool value) const
		{
			if (value)
			{
				m_buffer[m_byteIndex] |= (0x01 << m_bitIndex);
			}
			else
			{
				m_buffer[m_byteIndex] &= ~(0x01 << m_bitIndex);
			}
		}
		
		operator bool() const
		{
			char bit = (m_buffer[m_byteIndex] >> m_bitIndex) & 0x01;
			return bit == 1;
		}
		
	private:
		char* const m_buffer;
		const unsigned int m_byteIndex;
		const unsigned char m_bitIndex;
};

} // containers
} // flat

#endif // FLAT_CONTAINERS_BITACCESSOR



