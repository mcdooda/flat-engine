#ifndef FLAT_THREADS_THREADS_H
#define FLAT_THREADS_THREADS_H

#include <thread>
#include <tuple>
#include <vector>
#include <memory>

namespace flat
{
namespace threads
{

class Task
{
	public:
		virtual ~Task();
		
		virtual void execute() = 0;
};

template <class T, class It, typename... Args>
class TaskBatch : public Task
{
	public:
		TaskBatch(It begin, It end, void(T::*callback)(Args...), Args... args) :
			m_begin(begin),
			m_end(end),
			m_callback(callback),
			m_args(args...)
		{
		}

		void execute() override
		{
			for (It it = m_begin; it != m_end; ++it)
			{
				T* object = *it;
				call(object, std::index_sequence_for<Args...>{});
			}
		}

	private:
		template <size_t... Is>
		inline void call(T* object, std::index_sequence<Is...>)
		{
			(object->*m_callback)(std::get<Is>(m_args)...);
		}

	private:
		It m_begin;
		It m_end;
		void (T::*m_callback)(Args...);
		std::tuple<Args...> m_args;
};

class Threads
{
	public:
		static constexpr int INVALID_THREAD_INDEX = -1;
		static constexpr int MAIN_THREAD_INDEX = -2;

	public:
		Threads(int numThreads = std::thread::hardware_concurrency());

		template <class T, class It, class... Args>
		void schedule(It begin, It end, void(T::*callback)(Args...), Args... args);

		void execute();

		inline int getNumThreads() const { return static_cast<int>(m_tasksPerThread.size()); }

		static inline int getCurrentThreadIndex() { return currentThreadIndex; }
		static inline bool isMainThread() { return currentThreadIndex == MAIN_THREAD_INDEX; }

	private:
		static thread_local int currentThreadIndex;

		using TaskList = std::vector<std::unique_ptr<Task>>;
		std::vector<TaskList> m_tasksPerThread;
		int m_nextThreadIndex;
};


template<class T, class It, class ...Args>
inline void Threads::schedule(It begin, It end, void(T::*callback)(Args...), Args ...args)
{
	const int numElements = static_cast<int>(std::distance(begin, end));
	const int numThreads = static_cast<int>(m_tasksPerThread.size());
	const int numElementsPerThread = numElements / numThreads;
	const int numRemainingElements = numElements % numThreads;
	It currentElementIt = begin;
	for (int i = 0; i < numThreads; ++i)
	{
		int threadIndex = (m_nextThreadIndex + i) % numThreads;
		TaskList& taskList = m_tasksPerThread[i];
		int numElements = numElementsPerThread;
		if (i < numRemainingElements)
		{
			++numElements;
		}
		It threadBegin = currentElementIt;
		currentElementIt += numElements;
		It threadEnd = currentElementIt;
		taskList.emplace_back(
			std::move(
				std::make_unique<TaskBatch<T, It, Args...>>(threadBegin, threadEnd, callback, args...)
			)
		);
	}
	m_nextThreadIndex += numRemainingElements;
}

} // threads
} // flat

#endif // FLAT_THREADS_THREADS_H


