#include <dirent.h>
#include <cstring>
#include <algorithm>
#include "directory.h"

namespace flat
{
namespace file
{

Directory::Directory(const std::string& path) :
	File(path),
	m_read(false)
{

}

Directory::~Directory()
{
	std::for_each(m_content.begin(), m_content.end(), [](File* f) { delete f; });
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

bool Directory::isFileName(const std::string& name)
{
	int i;
	for (i = name.size() - 1; i >= 0 && name[i] != '.' && name[i] != '/'; i--);

	if (name[i] == '.')
		return true;

	else
		return false;
}

void Directory::getFiles(std::vector<File*>& files) const
{
	files.clear();
	
	std::for_each(m_content.begin(), m_content.end(),
		[&files](File* f)
		{
			if (f->isFile())
				files.push_back(f);
		}
	);
}

void Directory::getDirectories(std::vector<Directory*>& directories) const
{
	directories.clear();
	
	std::for_each(m_content.begin(), m_content.end(),
		[&directories](File* f)
		{
			if (f->isFile())
				directories.push_back(static_cast<Directory*>(f));
		}
	);
}

} // file
} // flat


