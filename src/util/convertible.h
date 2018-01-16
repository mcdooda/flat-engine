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
			FLAT_ASSERT_MSG(dynamic_cast<T*>(base) != nullptr, "Cannot convert from %s to %s", typeid(base).name(), typeid(T*).name());
			return static_cast<T*>(base);
		}

		template <class T>
		inline const T* toP() const
		{
			static_assert(std::is_base_of<Base, T>::value, "T must inherit from Base");
			const Base* base = static_cast<const Base*>(this);
			FLAT_ASSERT_MSG(dynamic_cast<const T*>(base) != nullptr, "Cannot convert from %s to %s", typeid(base).name(), typeid(const T*).name());
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

		// test type
		template <class T>
		inline bool is() const
		{
			const Base* base = static_cast<const Base*>(this);
			return dynamic_cast<const T*>(base) != nullptr;
		}
};

} // util
} // flat

#endif // FLAT_UTIL_CONVERTIBLE_H


