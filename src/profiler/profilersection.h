#ifndef FLAT_PROFILER_PROFILERSECTION_H
#define FLAT_PROFILER_PROFILERSECTION_H

#ifdef FLAT_PROFILER_ENABLED

#include "profiler.h"

namespace flat
{
namespace profiler
{

class ProfilerSection
{
	friend class Profiler;
	public:
		ProfilerSection() = delete;
		ProfilerSection(const char* name);
		ProfilerSection(const ProfilerSection&) = delete;
		ProfilerSection(ProfilerSection&&) = delete;
		~ProfilerSection();

		void operator=(const ProfilerSection&) = delete;
		void operator=(ProfilerSection&&) = delete;

	protected:
		const char* m_name;
		Profiler::TimePoint m_startTime;
};

} // profiler
} // flat

#endif // FLAT_PROFILER_ENABLED

#endif // FLAT_PROFILER_PROFILERSECTION_H


