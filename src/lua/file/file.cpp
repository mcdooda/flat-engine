#include "file.h"
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
		return std::make_shared<File>(path);
	case std::filesystem::file_type::directory:
		return std::make_shared<Directory>(path);
	default:
		return nullptr;
	}
}

} // file
} // lua
} // flat