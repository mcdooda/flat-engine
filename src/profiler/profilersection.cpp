#ifdef FLAT_PROFILER_ENABLED

#include "profiler/profilersection.h"
#include "profiler/profiler.h"

namespace flat
{
namespace profiler
{

ProfilerSection::ProfilerSection() :
	m_name(nullptr)
{
	m_startTime = std::chrono::high_resolution_clock::now();
	Profiler::getInstance().startSection(this);
}

ProfilerSection::ProfilerSection(const char* name) : ProfilerSection()
{
	m_name = name;
}

ProfilerSection::~ProfilerSection()
{
	Profiler::getInstance().endSection(this);
}

} // profiler
} // flat

#endif // FLAT_PROFILER_ENABLED