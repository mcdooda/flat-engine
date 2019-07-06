#ifndef FLAT_PROFILER_BINARYFORMAT_H
#define FLAT_PROFILER_BINARYFORMAT_H

#ifdef FLAT_PROFILER_ENABLED

#include <map>
#include <fstream>

#include "profiler/profiler.h"

namespace flat
{
namespace profiler
{

class BinaryWriter
{
	using SectionId = std::uint16_t;

	enum class Code : std::uint8_t
	{
		PUSH_SECTION,
		POP_SECTION,
		SECTION_NAMES
	};

	public:
		BinaryWriter() = delete;
		BinaryWriter(const std::string& fileName);
		BinaryWriter(const BinaryWriter&) = delete;
		BinaryWriter(BinaryWriter&&) = delete;
		~BinaryWriter() = default;

		void operator=(const BinaryWriter&) = delete;
		void operator=(const BinaryWriter&&) = delete;

		void pushSection(const char* name, Profiler::TimePoint startTime);
		void popSection(Profiler::TimePoint endTime);

		void writeSectionNames();

	private:
		SectionId getSectionId(const char* name);

		template <class T>
		inline void write(T value);

	private:
		std::map<const char*, SectionId> m_sectionIdsByName;
		std::ofstream m_file;
};

template<class T>
inline void BinaryWriter::write(T value)
{
	static_assert(std::is_pod<T>::value, "Generic implementation only for pod types");
	m_file.write(reinterpret_cast<const char*>(&value), sizeof(T));
}

template <>
inline void BinaryWriter::write(const char* value)
{
	uint8_t size = static_cast<uint8_t>(std::strlen(value));
	write(size);
	m_file.write(value, size);
}

template <>
inline void BinaryWriter::write(Profiler::TimePoint value)
{
	write(value.time_since_epoch().count());
}

} // profiler
} // flat

#endif // FLAT_PROFILER_ENABLED

#endif // FLAT_PROFILER_BINARYFORMAT_H


