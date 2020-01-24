#ifndef FLAT_PROFILER_PROFILER_H
#define FLAT_PROFILER_PROFILER_H

#ifdef FLAT_PROFILER_ENABLED

#include <chrono>
#include <string>
#include <vector>

#include "util/singleton.h"

namespace flat
{
namespace profiler
{
class ProfilerSection;
class BinaryWriter;

class Profiler : public flat::util::Singleton<Profiler>
{
	public:
		using Clock = std::chrono::high_resolution_clock;
		using TimePoint = std::chrono::time_point<Clock>;
		using Duration = TimePoint::duration;

	public:
		Profiler() = default;
		Profiler(const Profiler&) = delete;
		Profiler(Profiler&&) = delete;
		~Profiler() = default;

		void operator=(const Profiler&) = delete;
		void operator=(Profiler&&) = delete;

		inline static TimePoint getCurrentTime() { return Clock::now(); }

		void startSection(const ProfilerSection* profilerSection);
		void endSection(const ProfilerSection* profilerSection);

		void startRecording();
		void stopRecording();

		void saveSectionName(const std::shared_ptr<std::string>& sectionName);

	private:
		void popStartedSections();

	private:
		std::vector<const ProfilerSection*> m_currentSections;
		BinaryWriter* m_binaryWriter;
		bool m_shouldWrite; // do not write anything until the initially started sections are finished

		std::vector<std::shared_ptr<std::string>> m_savedSectionNames;
};

} // profiler
} // flat

#define FLAT_INIT_PROFILER() flat::profiler::Profiler::createInstance()
#define FLAT_DEINIT_PROFILER() flat::profiler::Profiler::destroyInstance()
#define FLAT_PROFILE(sectionName) flat::profiler::ProfilerSection profilerSection(sectionName)
#define FLAT_PROFILE_RESOURCE_LOADING(resourceName, durationForWarning) flat::profiler::ResourceProfilerSection resourceProfilerSection(resourceName, durationForWarning)

#else

#define FLAT_INIT_PROFILER() {}
#define FLAT_DEINIT_PROFILER() {}
#define FLAT_PROFILE(sectionName) {}
#define FLAT_PROFILE_RESOURCE_LOADING(resourceName, durationForWarning) {}

#endif // FLAT_PROFILER_ENABLED

#endif // FLAT_PROFILER_PROFILER_H


