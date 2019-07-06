#ifndef FLAT_PROFILER_PROFILER_H
#define FLAT_PROFILER_PROFILER_H

#ifdef FLAT_PROFILER_ENABLED

#include <chrono>
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
		using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

	public:
		Profiler() = default;
		Profiler(const Profiler&) = delete;
		Profiler(Profiler&&) = delete;
		~Profiler() = default;

		void operator=(const Profiler&) = delete;
		void operator=(Profiler&&) = delete;

		void startSection(const ProfilerSection* profilerSection);
		void endSection(const ProfilerSection* profilerSection);

		void startRecording();
		void stopRecording();

	private:
		void popStartedSections();

	private:
		std::vector<const ProfilerSection*> m_currentSections;
		BinaryWriter* m_binaryWriter;
		bool m_shouldWrite; // do not write anything until the initially started sections are finished
};

} // profiler
} // flat

#define FLAT_INIT_PROFILER() flat::profiler::Profiler::createInstance()
#define FLAT_DEINIT_PROFILER() flat::profiler::Profiler::destroyInstance()
#define FLAT_PROFILE(sectionName) flat::profiler::ProfilerSection profilerSection(sectionName)

#else

#define FLAT_INIT_PROFILER() {}
#define FLAT_DEINIT_PROFILER() {}
#define FLAT_PROFILE(sectionName) {}

#endif // FLAT_PROFILER_ENABLED

#endif // FLAT_PROFILER_PROFILER_H


