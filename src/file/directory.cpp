#include "directory.h"
#include "debug/assert.h"

namespace flat
{
namespace file
{

Directory::Directory(const std::filesystem::path& path) : File(path)
{

}

bool Directory::isValid() const
{
	return std::filesystem::is_directory(m_path);
}

std::shared_ptr<Directory> Directory::open(const std::filesystem::path& path)
{
	return openAs<Directory>(path);
}

void Directory::eachSubFile(std::function<void(const std::shared_ptr<File>&)> callback) const
{
	FLAT_ASSERT(isValid());
	for (const std::filesystem::directory_entry& directoryEntry : std::filesystem::directory_iterator(m_path))
	{
		std::shared_ptr<File> file = File::open(directoryEntry.path());
		if (file != nullptr)
		{
			callback(file);
		}
	}
}

void Directory::getSubFiles(std::vector<std::shared_ptr<File>>& files) const
{
	files.clear();
	eachSubFile([&files](const std::shared_ptr<File>& file) { files.push_back(file); });
}

void Directory::eachSubFileRecursive(std::function<void(const std::shared_ptr<File>&)> callback) const
{
	FLAT_ASSERT(isValid());
	for (const std::filesystem::directory_entry& directoryEntry : std::filesystem::recursive_directory_iterator(m_path))
	{
		std::shared_ptr<File> file = File::open(directoryEntry.path());
		if (file != nullptr)
		{
			callback(file);
		}
	}
}

void Directory::getSubFilesRecursive(std::vector<std::shared_ptr<File>>& files) const
{
	files.clear();
	eachSubFileRecursive([&files](const std::shared_ptr<File>& file) { files.push_back(file); });
}

} // file
} // flat