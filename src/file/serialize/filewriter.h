#ifndef FLAT_FILE_SERIALIZE_FILEWRITER_H
#define FLAT_FILE_SERIALIZE_FILEWRITER_H

#include <fstream>

#include "processor.h"

namespace flat
{
namespace file
{
namespace serialize
{

class FileWriter final : public Processor
{
	public:
		FileWriter(const std::string& filename);

		bool canWrite() const;

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

		void process(flat::Vector2& value) override;
		void process(flat::Vector2i& value) override;
		void process(flat::Vector3& value) override;
		void process(flat::Vector3i& value) override;

		void process(std::string& value) override;
		void process(std::filesystem::path& value) override;

	private:
		template <class T>
		inline void write(T value);

	private:
		std::ofstream m_file;
};

template <class T>
void FileWriter::write(T value)
{
	static_assert(std::is_pod<T>::value, "Generic implementation only for pod types");
	m_file.write(reinterpret_cast<const char*>(&value), sizeof(T));
}

} // serialize
} // file
} // flat

#endif // FLAT_FILE_SERIALIZE_FILEWRITER_H