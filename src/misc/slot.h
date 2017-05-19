#ifndef FLAT_SLOT_H
#define FLAT_SLOT_H

#include <vector>
#include <algorithm>
#include "../memory/memory.h"

namespace flat
{

template <typename... T>
class Slot
{
	private:
		class Callback
		{
			public:
				virtual ~Callback() {}
				virtual bool operator()(T... params) = 0;
		};

		template <typename U>
		class CallbackMethodImpl : public Callback
		{
			public:
				using CallbackType = bool (U::*)(T... params);
			public:
				CallbackMethodImpl(U* object, CallbackType callbackMethod) :
					m_object(object),
					m_callbackMethod(callbackMethod) {}

				bool operator()(T... params) override
				{
					FLAT_ASSERT(*reinterpret_cast<uint8_t*>(m_object) != FLAT_WIPE_VALUE);
					return (m_object->*m_callbackMethod)(params...);
				}

			public:
				U* m_object;

			private:
				CallbackType m_callbackMethod;
		};

		template <typename Func>
		class CallbackFunctionImpl : public Callback
		{
			public:
				CallbackFunctionImpl(Func callbackFunc) :
					m_callbackFunc(callbackFunc) {}

				bool operator()(T... params) override
				{
					return m_callbackFunc(params...);
				}

			private:
				Func m_callbackFunc;
		};

	public:
		~Slot()
		{
			off();
		}

		void operator()(T... params)
		{
			for (std::vector<Callback*>::iterator it = m_callbacks.begin(); it != m_callbacks.end(); )
			{
				bool keepCallback = (**it)(params...);
				if (keepCallback)
				{
					++it;
				}
				else
				{
					it = m_callbacks.erase(it);
				}
			}
		}

		bool on() const
		{
			return !m_callbacks.empty();
		}

		// VS 2015 does not see this function this way
		//template <typename U>
		//void on(U* object, void (std::remove_const<U>::type::*callbackMethod)(T...))
		template <typename PointerType, typename MethodType>
		void on(PointerType* object, MethodType callbackMethod)
		{
			static_assert(std::is_same<MethodType, CallbackMethodImpl<PointerType>::CallbackType>::value, "Callback must be a method of the given object and return a boolean");
			FLAT_ASSERT(object != nullptr && callbackMethod != nullptr);
			Callback* callback = new CallbackMethodImpl<PointerType>(object, callbackMethod);
			m_callbacks.push_back(callback);
		}

		template <typename Func>
		void on(Func callbackFunc)
		{
			//FLAT_ASSERT(callbackFunc); // no operator! for lambdas
			Callback* callback = new CallbackFunctionImpl<Func>(callbackFunc);
			m_callbacks.emplace_back(callback);
		}

		template <typename U>
		void off(U* object)
		{
			for (std::vector<Callback*>::iterator it = m_callbacks.begin(); it != m_callbacks.end(); )
			{
				CallbackMethodImpl<U>* callbackMethodImpl = dynamic_cast<CallbackMethodImpl<U>*>(*it);
				if (callbackMethodImpl != nullptr && callbackMethodImpl->m_object == object)
				{
					delete callbackMethodImpl;
					it = m_callbacks.erase(it);
				}
				else
				{
					++it;
				}
			}
		}

		template <typename U>
		void off(U* object, void (U::*callbackMethod)(T...))
		{
			for (std::vector<Callback*>::iterator it = m_callbacks.begin(); it != m_callbacks.end(); )
			{
				CallbackMethodImpl<U>* callbackMethodImpl = dynamic_cast<CallbackMethodImpl<U>*>(*it);
				if (callbackMethodImpl != nullptr && callbackMethodImpl->m_object == object && callbackMethodImpl->m_callbackMethod == callbackMethod)
				{
					delete callback;
					it = m_callbacks.erase(it);
				}
				else
				{
					++it;
				}
			}
		}

		template <typename Func>
		void off(Func callbackFunc)
		{
			for (std::vector<Callback*>::iterator it = m_callbacks.begin(); it != m_callbacks.end(); ++it)
			{
				CallbackFunctionImpl<Func>* callbackFunctionImpl = dynamic_cast<CallbackFunctionImpl<Func>*>(*it);
				if (callbackFunctionImpl != nullptr && callbackFunctionImpl->m_callbackFunc == callbackFunc)
				{
					delete callback;
					it = m_callbacks.erase(it);
				}
				else
				{
					++it;
				}
			}
		}

		void off()
		{
			for (Callback* callback : m_callbacks)
			{
				FLAT_FREE(callback);
			}
			m_callbacks.clear();
		}

	private:
		std::vector<Callback*> m_callbacks;
};

} // flat

#endif // FLAT_SLOT_H


