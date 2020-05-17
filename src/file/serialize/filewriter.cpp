#include "filewriter.h"

namespace flat
{
namespace file
{
namespace serialize
{

FileWriter::FileWriter(const std::string& fileName) :
	m_file(fileName.c_str(), std::ofstream::binary)
{

}

bool FileWriter::canWrite() const
{
	return m_file.is_open();
}

bool FileWriter::isWriting() const
{
	return true;
}

void FileWriter::process(bool& value)
{
	write(value);
}

void FileWriter::process(std::int8_t& value)
{
	write(value);
}

void FileWriter::process(std::int16_t& value)
{
	write(value);
}

void FileWriter::process(std::int32_t& value)
{
	write(value);
}

void FileWriter::process(std::uint8_t& value)
{
	write(value);
}

void FileWriter::process(std::uint16_t& value)
{
	write(value);
}

void FileWriter::process(std::uint32_t& value)
{
	write(value);
}

void FileWriter::process(float& value)
{
	write(value);
}

void FileWriter::process(double& value)
{
	write(value);
}

void FileWriter::process(flat::Vector2& value)
{
	write(value.x);
	write(value.y);
}

void FileWriter::process(flat::Vector2i& value)
{
	write(value.x);
	write(value.y);
}

void FileWriter::process(flat::Vector3& value)
{
	write(value.x);
	write(value.y);
	write(value.z);
}

void FileWriter::process(flat::Vector3i& value)
{
	write(value.x);
	write(value.y);
	write(value.z);
}

void FileWriter::process(std::string& value)
{
	const std::uint16_t size = static_cast<std::uint16_t>(value.size());
	write<std::uint16_t>(size);
	m_file.write(value.data(), size);
}

void FileWriter::process(std::filesystem::path& value)
{
	std::string pathAsString = value.string();
	constexpr char separator = std::filesystem::path::preferred_separator;
	if constexpr (separator != '/')
	{
		std::replace(pathAsString.begin(), pathAsString.end(), separator, '/');
	}
	process(pathAsString);
}

} // serialize
} // file
} // flat