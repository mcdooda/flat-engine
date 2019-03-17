#include "file/regularfile.h"

namespace flat
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
} // flat