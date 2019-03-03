#include "regularfile.h"

namespace flat
{
namespace lua
{
namespace file
{

RegularFile::RegularFile(const std::filesystem::path& path) : File(path)
{

}

bool RegularFile::isValid() const
{
	return std::filesystem::is_regular_file(m_path);
}

std::shared_ptr<RegularFile> RegularFile::open(const std::filesystem::path& path)
{
	return openAs<RegularFile>(path);
}

} // file
} // lua
} // flat