#ifndef ENGINE_FILE_DIRECTORY_H
#define ENGINE_FILE_DIRECTORY_H

#include <vector>
#include "file.h"

namespace engine
{
namespace file
{

class Directory : public File
{
	public:
		Directory(std::string path);
		virtual ~Directory();

		void read();

		static bool isFileName(std::string name);

		virtual inline bool isFile() const { return false; }
		virtual inline bool isDir() const { return true; }

		inline const std::vector<File*>& getContent() const { return m_content; }
		std::vector<File*> getFiles();
		std::vector<Directory*> getDirectories();

	private:
		std::vector<File*> m_content;
		bool m_read;
};

} // file
} // engine

#endif // ENGINE_FILE_DIRECTORY_H


