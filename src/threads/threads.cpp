#include <future>
#include "threads.h"

namespace flat
{
namespace threads
{

thread_local int Threads::currentThreadIndex = Threads::INVALID_THREAD_INDEX;

Task::~Task()
{
}

Threads::Threads(int numThreads) :
	m_nextThreadIndex(0)
{
	m_tasksPerThread.resize(numThreads);

	currentThreadIndex = MAIN_THREAD_INDEX;
}

void Threads::execute()
{
	const int numThreads = static_cast<int>(m_tasksPerThread.size());

#define ENABLE_MULTITHREADING 1

#if ENABLE_MULTITHREADING
	std::vector<std::future<void>> futures(numThreads);
#endif // ENABLE_MULTITHREADING

	for (int i = 0; i < numThreads; ++i)
	{
#if ENABLE_MULTITHREADING
		futures[i] = std::async(
			std::launch::async,
			[this, i]()
			{
#endif // ENABLE_MULTITHREADING
				currentThreadIndex = i;
				TaskList& taskList = m_tasksPerThread[i];
				for (std::unique_ptr<Task>& task : taskList)
				{
					task->execute();
				}
				currentThreadIndex = INVALID_THREAD_INDEX;
#if ENABLE_MULTITHREADING
			}
		);
#endif // ENABLE_MULTITHREADING
	}

#if ENABLE_MULTITHREADING
	for (int i = 0; i < numThreads; ++i)
	{
		futures[i].get();
	}
#else
	currentThreadIndex = MAIN_THREAD_INDEX;
#endif // ENABLE_MULTITHREADING

	for (TaskList& taskList : m_tasksPerThread)
	{
		taskList.clear();
	}

#undef ENABLE_MULTITHREADING
}

} // threads
} // flat

