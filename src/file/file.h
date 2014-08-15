#ifndef ENGINE_FILE_FILE_H
#define ENGINE_FILE_FILE_H

#include <string>

namespace engine
{
namespace file
{

class File
{
	public:
		File(std::string path);
		virtual ~File();

		inline const std::string& getPath() const { return m_path; }

		virtual inline bool isFile() const { return true; }
		virtual inline bool isDir() const { return false; }

		bool isReadable();
		bool isWritable();

	protected:
		std::string m_path;
};

} // file
} // engine

#endif // ENGINE_FILE_FILE_H


