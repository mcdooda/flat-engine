#ifndef FLAT_PROFILER_RESOURCEPROFILERSECTION_H
#define FLAT_PROFILER_RESOURCEPROFILERSECTION_H

#ifdef FLAT_PROFILER_ENABLED

#include "profiler/profilersection.h"

namespace flat
{
namespace profiler
{

class ResourceProfilerSection : public ProfilerSection
{
	public:
		ResourceProfilerSection() = delete;
		ResourceProfilerSection(const char* name, Profiler::Duration durationForWarning);
		ResourceProfilerSection(const ResourceProfilerSection&) = delete;
		ResourceProfilerSection(ResourceProfilerSection&&) = delete;
		virtual ~ResourceProfilerSection() override;

	private:
		Profiler::Duration m_durationForWarning;
};

} // profiler
} // flat

#endif // FLAT_PROFILER_ENABLED

#endif // FLAT_PROFILER_RESOURCEPROFILERSECTION_H


