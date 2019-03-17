#ifndef FLAT_FILE_DIRECTORY_H
#define FLAT_FILE_DIRECTORY_H

#include <vector>
#include <functional>

#include "file/file.h"

namespace flat
{
namespace file
{

class Directory : public File
{
	public:
		Directory(const std::filesystem::path& path);

		bool isValid() const override;

		static std::shared_ptr<Directory> open(const std::filesystem::path& path);

		void eachSubFile(std::function<void(const std::shared_ptr<File>&)> callback) const;
		void getSubFiles(std::vector<std::shared_ptr<File>>& files) const;

		void eachSubFileRecursive(std::function<void(const std::shared_ptr<File>&)> callback) const;
		void getSubFilesRecursive(std::vector<std::shared_ptr<File>>& files) const;
};

} // file
} // flat

#endif // FLAT_FILE_DIRECTORY_H