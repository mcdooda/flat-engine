#include "file.h"
#include "regularfile.h"
#include "directory.h"

namespace flat
{
namespace lua
{
namespace file
{

File::File(const std::filesystem::path& path) :
	m_path(path)
{

}

std::string File::getShortStem() const
{
	std::string stem = m_path.stem().string();
	size_t firstDotIndex = stem.find_first_of('.');
	if (firstDotIndex == std::string::npos)
	{
		return stem;
	}
	return stem.substr(0, firstDotIndex);
}

std::string File::getFullExtension() const
{
	std::string filename = m_path.filename().string();
	if (filename == "." || filename == "..")
	{
		return "";
	}
	size_t firstDotIndex = filename.find_first_of('.');
	if (firstDotIndex == std::string::npos)
	{
		return "";
	}
	return filename.substr(firstDotIndex);
}

std::filesystem::file_type File::getType() const
{
	return std::filesystem::status(m_path).type();
}

bool File::isValid() const
{
	return std::filesystem::exists(m_path);
}

std::shared_ptr<File> File::open(const std::filesystem::path& path)
{
	switch (std::filesystem::status(path).type())
	{
	case std::filesystem::file_type::regular:
		return std::make_shared<RegularFile>(path);
	case std::filesystem::file_type::directory:
		return std::make_shared<Directory>(path);
	default:
		return nullptr;
	}
}

std::string File::formatPath(const std::filesystem::path& path)
{
	std::string formattedPath = path.string();
	constexpr char separator = std::filesystem::path::preferred_separator;
	if constexpr (separator != '/')
	{
		std::replace(formattedPath.begin(), formattedPath.end(), separator, '/');
	}
	return formattedPath;
}

} // file
} // lua
} // flat