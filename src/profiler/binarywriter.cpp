#include <vector>

#include "binarywriter.h"

#include "../debug/assert.h"

namespace flat
{
namespace profiler
{

BinaryWriter::BinaryWriter(const std::string& fileName) :
	m_file(fileName, std::ofstream::binary)
{
	FLAT_ASSERT(m_file.is_open());
}

BinaryWriter::~BinaryWriter()
{
	writeSectionNames();
}

void BinaryWriter::pushSection(const char* name, Profiler::TimePoint startTime)
{
	write(Code::PUSH_SECTION);
	write(getSectionId(name));
	write(startTime);
}

void BinaryWriter::popSection(Profiler::TimePoint endTime)
{
	write(Code::POP_SECTION);
	write(endTime);
}

BinaryWriter::SectionId BinaryWriter::getSectionId(const char* name)
{
	std::map<const char*, SectionId>::iterator it = m_sectionIdsByName.find(name);
	if (it == m_sectionIdsByName.end())
	{
		SectionId sectionId = static_cast<SectionId>(m_sectionIdsByName.size());
		m_sectionIdsByName[name] = sectionId;
		return sectionId;
	}
	else
	{
		return it->second;
	}
}

void BinaryWriter::writeSectionNames()
{
	std::vector<const char*> sectionNames(m_sectionIdsByName.size());
	for (const std::pair<const char*, SectionId>& sectionName : m_sectionIdsByName)
	{
		sectionNames[sectionName.second] = sectionName.first;
	}

	write(Code::SECTION_NAMES);
	for (const char* sectionName : sectionNames)
	{
		write(sectionName);
	}
}

} // profiler
} // flat


