#ifndef FLAT_FILE_DIRECTORY_H
#define FLAT_FILE_DIRECTORY_H

#include <vector>
#include "file.h"

namespace flat
{
namespace file
{

class Directory : public File
{
	public:
		Directory(const std::string& path);
		~Directory() override;

		void read();

		static bool isFileName(const std::string& name);

		bool isFile() const override { return false; }
		bool isDir() const override { return true; }

		inline const std::vector<File*>& getContent() const { return m_content; }
		void getFiles(std::vector<File*>& files) const;
		void getDirectories(std::vector<Directory*>& directories) const;

	private:
		std::vector<File*> m_content;
		bool m_read;
};

} // file
} // flat

#endif // FLAT_FILE_DIRECTORY_H


