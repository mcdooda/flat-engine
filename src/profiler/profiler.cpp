#include "profiler.h"

#include "binarywriter.h"
#include "profilersection.h"

#include "../debug/assert.h"
#include "../memory/memory.h"

namespace flat
{
namespace profiler
{

void Profiler::startSection(const ProfilerSection* profilerSection)
{
	m_currentSections.push_back(profilerSection);
	if (m_binaryWriter != nullptr)
	{
		m_binaryWriter->pushSection(profilerSection->m_name, profilerSection->m_startTime);
	}
}

void Profiler::endSection(const ProfilerSection* profilerSection)
{
	FLAT_ASSERT(profilerSection == m_currentSections.back());
	m_currentSections.pop_back();
	if (m_binaryWriter != nullptr)
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();
		m_binaryWriter->popSection(end);
	}
}

void Profiler::startRecording()
{
	m_binaryWriter = new BinaryWriter("profile.fp");
	pushStartedSections();
}

void Profiler::stopRecording()
{
	popStartedSections();
	FLAT_DELETE(m_binaryWriter);
}

void Profiler::pushStartedSections()
{
	FLAT_ASSERT(m_binaryWriter != nullptr);
	for (const ProfilerSection* profilerSection : m_currentSections)
	{
		m_binaryWriter->pushSection(profilerSection->m_name, profilerSection->m_startTime);
	}
}

void Profiler::popStartedSections()
{
	FLAT_ASSERT(m_binaryWriter != nullptr);
	std::chrono::time_point<std::chrono::high_resolution_clock> interruptedTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < m_currentSections.size(); ++i)
	{
		m_binaryWriter->popSection(interruptedTime);
	}
}

} // profiler
} // flat


