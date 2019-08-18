#include "filereader.h"

namespace flat
{
namespace file
{
namespace serialize
{

FileReader::FileReader(const std::string& filename) :
	m_file(filename.c_str(), std::ofstream::binary)
{

}

bool FileReader::canRead() const
{
	return m_file.is_open();
}

bool FileReader::isWriting() const
{
	return false;
}

void FileReader::process(bool& value)
{
	read(value);
}

void FileReader::process(std::int8_t& value)
{
	read(value);
}

void FileReader::process(std::int16_t& value)
{
	read(value);
}

void FileReader::process(std::int32_t& value)
{
	read(value);
}

void FileReader::process(std::uint8_t& value)
{
	read(value);
}

void FileReader::process(std::uint16_t& value)
{
	read(value);
}

void FileReader::process(std::uint32_t& value)
{
	read(value);
}

void FileReader::process(float& value)
{
	read(value);
}

void FileReader::process(double& value)
{
	read(value);
}

void FileReader::process(std::string& value)
{
	uint16_t size;
	read<uint16_t>(size);
	value.resize(size);
	m_file.read(&value[0], size);
}

} // serialize
} // file
} // flat