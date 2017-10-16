#include "networkprocessor.h"

namespace flat
{
namespace network
{
namespace common
{

bool NetworkReader::processDelimiter(Delimiter delimiter) const
{
	Delimiter value;
	read(value);
	return value == delimiter;
}
bool NetworkWriter::processDelimiter(Delimiter delimiter) const
{
	return write(delimiter);
}


bool NetworkReader::processValue(bool& value) const
{
	return read(value);
}
bool NetworkWriter::processValue(bool& value) const
{
	return write(value);
}


bool NetworkReader::processValue(std::uint8_t& value) const
{
	return read(value);
}
bool NetworkWriter::processValue(std::uint8_t& value) const
{
	return write(value);;
}


bool NetworkReader::processValue(std::uint16_t& value) const
{
	return read(value);
}
bool NetworkWriter::processValue(std::uint16_t& value) const
{
	return write(value);
}


bool NetworkReader::processValue(std::uint32_t& value) const
{
	return read(value);
}
bool NetworkWriter::processValue(std::uint32_t& value) const
{
	return write(value);
}


bool NetworkReader::processValue(std::int8_t& value) const
{
	return read(value);
}
bool NetworkWriter::processValue(std::int8_t& value) const
{
	return write(value);
}


bool NetworkReader::processValue(std::int16_t& value) const
{
	return read(value);
}
bool NetworkWriter::processValue(std::int16_t& value) const
{
	return write(value);
}


bool NetworkReader::processValue(std::int32_t& value) const
{
	return read(value);
}
bool NetworkWriter::processValue(std::int32_t& value) const
{
	return write(value);
}


bool NetworkReader::processValue(Vector2& value) const
{
	return read(value);
}
bool NetworkWriter::processValue(Vector2& value) const
{
	return write(value);
}


bool NetworkReader::processValue(Vector3& value) const
{
	return read(value);
}
bool NetworkWriter::processValue(Vector3& value) const
{
	return write(value);
}


bool NetworkReader::processValue(Vector4& value) const
{
	return read(value);
}
bool NetworkWriter::processValue(Vector4& value) const
{
	return write(value);
}

} // common
} // network
} // flat


