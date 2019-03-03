#ifndef FLAT_LUA_FILE_FILE_H
#define FLAT_LUA_FILE_FILE_H

#include <filesystem>
#include <memory.h>

#include "../../util/convertible.h"

namespace flat
{
namespace lua
{
namespace file
{

class File : public util::Convertible<File>
{
	public:
		File(const std::filesystem::path& path);

		inline std::string getPath() const { return formatPath(m_path); }
		inline std::string getParentPath() const { return formatPath(m_path.parent_path()); }
		inline std::string getFileName() const { return m_path.filename().string(); }
		inline std::string getStem() const { return m_path.stem().string(); }
		inline std::string getExtension() const { return m_path.extension().string(); }

		std::string getShortStem() const;
		std::string getFullExtension() const;

		std::filesystem::file_type getType() const;

		virtual bool isValid() const;

		static std::shared_ptr<File> open(const std::filesystem::path& path);

		template <class T>
		static std::shared_ptr<T> openAs(const std::filesystem::path& path);

	protected:
		static std::string formatPath(const std::filesystem::path& path);

	protected:
		std::filesystem::path m_path;
};

template <class T>
std::shared_ptr<T> File::openAs(const std::filesystem::path& path)
{
	std::shared_ptr<T> file = std::make_shared<T>(path);
	if (file->isValid())
	{
		return file;
	}
	return nullptr;
}

} // file
} // lua
} // flat

#endif // FLAT_LUA_FILE_FILE_H