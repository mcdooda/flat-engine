#ifndef FLAT_UTIL_SINGLETON_H
#define FLAT_UTIL_SINGLETON_H

#include <memory>

#include "debug/assert.h"

namespace flat
{
namespace util
{

template <class T>
class Singleton
{
	public:
		virtual ~Singleton()
		{
			FLAT_ASSERT_MSG(instanceDestroyedProperly, "Instance has not be destroyed properly, call destroyInstance() explicitly");
		}

		static void createInstance()
		{
			FLAT_ASSERT_MSG(instance == nullptr, "Instance already exists");
			instance = std::make_unique<T>();
			FLAT_DEBUG_ONLY(instanceDestroyedProperly = false;)
		}

		static void destroyInstance()
		{
			FLAT_DEBUG_ONLY(instanceDestroyedProperly = true;)
			instance.reset();
		}

		static T& getInstance()
		{
			FLAT_ASSERT_MSG(instance != nullptr, "Instance has not been created, call createInstance() before");
			return *instance.get();
		}

	private:
		static std::unique_ptr<T> instance;
		FLAT_DEBUG_ONLY(static bool instanceDestroyedProperly;)
};

template <class T>
std::unique_ptr<T> Singleton<T>::instance;

#ifdef FLAT_DEBUG
template <class T>
bool Singleton<T>::instanceDestroyedProperly = false;
#endif

} // util
} // flat

#endif // FLAT_UTIL_SINGLETON_H


