#include <dirent.h>
#include <cstring>
#include <string>
#include "directory.h"

namespace engine
{
namespace file
{

Directory::Directory(std::string path) :
	File(path),
	m_read(false)
{

}

Directory::~Directory()
{
	std::vector<File*>::iterator end = m_content.end();
	for (std::vector<File*>::iterator it = m_content.begin(); it != end; it++)
		delete *it;
}

void Directory::read()
{
	if (!m_read)
	{
		dirent* entry;
		DIR* d = opendir(m_path.c_str());

		while ((entry = readdir(d)))
		{
			if (entry->d_name[0] != '.')
			{
				std::string path = m_path + "/" + entry->d_name;
				if (isFileName(entry->d_name))
				{
					File* f = new File(path);
					m_content.push_back(f);
				}
				else
				{
					Directory* dir = new Directory(path);
					dir->read();
					m_content.push_back(dir);
				}
			}
		}

		closedir(d);
	}
}

bool Directory::isFileName(std::string name)
{
	int i;
	for (i = name.size() - 1; i >= 0 && name[i] != '.' && name[i] != '/'; i--);

	if (name[i] == '.')
		return true;

	else
		return false;
}

std::vector<File*> Directory::getFiles()
{
	std::vector<File*> files;

	std::vector<File*>::iterator end = m_content.end();
	for (std::vector<File*>::iterator it = m_content.begin(); it != end; it++)
		if ((*it)->isFile())
			files.push_back(*it);

	return files;
}

std::vector<Directory*> Directory::getDirectories()
{
	std::vector<Directory*> dirs;

	std::vector<File*>::iterator end = m_content.end();
	for (std::vector<File*>::iterator it = m_content.begin(); it != end; it++)
		if ((*it)->isDir())
			dirs.push_back((Directory*) *it);

	return dirs;
}

} // file
} // engine


