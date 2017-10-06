#ifndef FLAT_UTIL_INSTANCER_H
#define FLAT_UTIL_INSTANCER_H

#include "../containers/dynamicpool.h"

namespace flat
{
namespace util
{

template <class Base>
class InstancerProxy
{
	public:
		virtual Base* makeObject() const = 0;
		virtual Base* makeObject(containers::DynamicPool& pool) const = 0;
};

template <class Base, class Derived>
class DerivedInstancerProxy : public InstancerProxy<Base>
{
	public:
		Base* makeObject() const override
		{
			return new Derived();
		}

		Base* makeObject(containers::DynamicPool& pool) const override
		{
			return pool.create<Derived>();
		}
};

template <class Base>
class Instancer
{
	private:
		using Proxy = InstancerProxy<Base>;

	public:
		Instancer() :
			m_proxy(nullptr)
		{
			
		}

		~Instancer()
		{
			const Proxy* proxy = getProxy();
			if (proxy != nullptr)
			{
				proxy->~Proxy();
			}
		}

		template <class Derived>
		inline void makeInstancer()
		{
			static_assert(std::is_base_of<Base, Derived>::value, "Derived must derive from Base");
			FLAT_ASSERT(m_proxy == nullptr);
			using DerivedProxy = DerivedInstancerProxy<Base, Derived>;
			FLAT_ASSERT(sizeof(DerivedProxy) <= sizeof(m_proxy));
			new (reinterpret_cast<DerivedProxy*>(&m_proxy)) DerivedProxy();
		}

		inline Base* makeObject() const
		{
			const Proxy* proxy = getProxy();
			return proxy->makeObject();
		}

		inline Base* makeObject(containers::DynamicPool& pool) const
		{
			const Proxy* proxy = getProxy();
			return proxy->makeObject(pool);
		}

	private:
		inline const Proxy* getProxy() const
		{
			FLAT_ASSERT(m_proxy != nullptr);
			return reinterpret_cast<const Proxy*>(&m_proxy);
		}

	private:
		void* m_proxy;
};

} // util
} // flat

#endif // FLAT_UTIL_INSTANCER_H


