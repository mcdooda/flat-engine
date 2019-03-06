#ifndef FLAT_FILE_REGULARFILE_H
#define FLAT_FILE_REGULARFILE_H

#include "file/file.h"

namespace flat
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
} // flat

#endif // FLAT_FILE_REGULARFILE_H