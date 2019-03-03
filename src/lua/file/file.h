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

		inline const char* getPath() const { return reinterpret_cast<const char*>(m_path.c_str()); }

		std::filesystem::file_type getType() const;

		virtual bool isValid() const;

		static std::shared_ptr<File> open(const std::filesystem::path& path);

		template <class T>
		static std::shared_ptr<T> openAs(const std::filesystem::path& path);

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