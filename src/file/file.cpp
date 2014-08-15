#include <fstream>
#include <cstring>
#include "file.h"

namespace flat
{
namespace file
{

File::File(std::string path) :
	m_path(path)
{

}

File::~File()
{
	
}

bool File::isReadable()
{
	std::ifstream f(m_path.c_str());
	bool readable = false;
	if (f)
		readable = true;

	f.close();

	return readable;
}

bool File::isWritable()
{
	std::ofstream f(m_path.c_str());
	bool writable = false;
	if (f)
		writable = true;

	f.close();

	return writable;
}

} // file
} // flat


