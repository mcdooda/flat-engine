#ifndef FLAT_FILE_FILE_H
#define FLAT_FILE_FILE_H

#include <string>

namespace flat
{
namespace file
{

class File
{
	public:
		File(const std::string& path);
		virtual ~File();

		inline const std::string& getPath() const { return m_path; }

		virtual bool isFile() const { return true; }
		virtual bool isDir() const { return false; }

		bool isReadable() const;
		bool isWritable() const;

	protected:
		const std::string m_path;
};

} // file
} // flat

#endif // FLAT_FILE_FILE_H


