#ifndef FLAT_UTIL_CONVERTIBLE_H
#define FLAT_UTIL_CONVERTIBLE_H

namespace flat
{
namespace util
{

template <class Base>
class Convertible
{
	public:
		// pointer
		template <class T>
		inline T* toP()
		{
			static_assert(std::is_base_of<Base, T>::value, "T must inherit from Base");
			Base* base = static_cast<Base*>(this);
			FLAT_ASSERT(dynamic_cast<T*>(base) != nullptr);
			return static_cast<T*>(base);
		}

		template <class T>
		inline const T* toP() const
		{
			static_assert(std::is_base_of<Base, T>::value, "T must inherit from Base");
			const Base* base = static_cast<const Base*>(this);
			FLAT_ASSERT(dynamic_cast<const T*>(base) != nullptr);
			return static_cast<const T*>(base);
		}

		// unsafe cast
		template <class T>
		inline T* asP()
		{
			return static_cast<T*>(this);
		}

		template <class T>
		inline const T* asP() const
		{
			return static_cast<const T*>(this);
		}

		// reference
		template <class T>
		inline T& to()
		{
			return *toP<T>();
		}

		template <class T>
		inline const T& to() const
		{
			return *toP<T>();
		}

		// unsafe cast
		template <class T>
		inline T& as()
		{
			return *asP<T>();
		}

		template <class T>
		inline const T& as() const
		{
			return *asP<T>();
		}
};

} // util
} // flat

#endif // FLAT_UTIL_CONVERTIBLE_H


