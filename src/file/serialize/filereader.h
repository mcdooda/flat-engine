#ifndef FLAT_FILE_SERIALIZE_FILEREADER_H
#define FLAT_FILE_SERIALIZE_FILEREADER_H

#include <fstream>

#include "processor.h"

namespace flat
{
namespace file
{
namespace serialize
{

class FileReader : public Processor
{
	public:
		FileReader(const std::string& filename);

		bool canRead() const;

		bool isWriting() const override;

		void process(bool& value) override;

		void process(std::int8_t& value) override;
		void process(std::int16_t& value) override;
		void process(std::int32_t& value) override;
		void process(std::uint8_t& value) override;
		void process(std::uint16_t& value) override;
		void process(std::uint32_t& value) override;

		void process(float& value) override;
		void process(double& value) override;

		void process(std::string& value) override;

	private:
		template <class T>
		inline void read(T& value);

	private:
		std::ifstream m_file;
};

template <class T>
void FileReader::read(T& value)
{
	static_assert(std::is_pod<T>::value, "Generic implementation only for pod types");
	m_file.read(reinterpret_cast<char*>(&value), sizeof(T));
}

} // serialize
} // file
} // flat

#endif // FLAT_FILE_SERIALIZE_FILEREADER_H