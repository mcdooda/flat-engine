#ifndef FLAT_LUA_FILE_REGULARFILE_H
#define FLAT_LUA_FILE_REGULARFILE_H

#include "file.h"

namespace flat
{
namespace lua
{
namespace file
{

class RegularFile : public File
{
	public:
		RegularFile(const std::filesystem::path& path);

		bool isValid() const override;

		static std::shared_ptr<RegularFile> open(const std::filesystem::path& path);
};

} // file
} // lua
} // flat

#endif // FLAT_LUA_FILE_REGULARFILE_H