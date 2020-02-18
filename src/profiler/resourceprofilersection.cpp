#ifdef FLAT_PROFILER_ENABLED

#include <iostream>

#include "profiler/resourceprofilersection.h"

#include "console/console.h"

namespace flat
{
namespace profiler
{

ResourceProfilerSection::ResourceProfilerSection(const char* name, Profiler::Duration durationForWarning) : ProfilerSection(),
	m_durationForWarning(durationForWarning)
{
	std::shared_ptr<std::string> savedSectionName = std::make_shared<std::string>(name);
	Profiler::getInstance().saveSectionName(savedSectionName);
	m_name = savedSectionName->c_str();
}

ResourceProfilerSection::~ResourceProfilerSection()
{
	Profiler::TimePoint endTime = Profiler::getCurrentTime();
	Profiler::Duration duration = endTime - m_startTime;
	if (duration > m_durationForWarning)
	{
		FLAT_CONSOLE_COLOR(LIGHT_BLUE);
		std::cerr << "Resource '" << m_name << "' took " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "ms to load" << std::endl;
	}
}

} // profiler
} // flat

#endif // FLAT_PROFILER_ENABLED