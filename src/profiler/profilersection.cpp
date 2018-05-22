#include "profilersection.h"

#include "profiler.h"

namespace flat
{
namespace profiler
{

ProfilerSection::ProfilerSection(const char* name) :
	m_name(name)
{
	m_startTime = std::chrono::high_resolution_clock::now();
	Profiler::getInstance().startSection(this);
}

ProfilerSection::~ProfilerSection()
{
	Profiler::getInstance().endSection(this);
}

} // profiler
} // flat


