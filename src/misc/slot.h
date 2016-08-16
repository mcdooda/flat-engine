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
				virtual void operator()(T... params) = 0;
		};

		template <typename U>
		class CallbackMethodImpl : public Callback
		{
			public:
				CallbackMethodImpl(U* object, void (U::*callbackMethod)(T... params)) :
					m_object(object),
					m_callbackMethod(callbackMethod) {}

				void operator()(T... params) override
				{
					(m_object->*m_callbackMethod)(params...);
				}

			private:
				U* m_object;
				void (U::*m_callbackMethod)(T...);
		};

		template <typename Func>
		class CallbackFunctionImpl : public Callback
		{
			public:
				CallbackFunctionImpl(Func callbackFunc) :
					m_callbackFunc(callbackFunc) {}

				void operator()(T... params) override
				{
					m_callbackFunc(params...);
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
			for (Callback* callback : m_callbacks)
				(*callback)(params...);
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
			static_assert(std::is_same<MethodType, void (std::remove_const<PointerType>::type::*)(T...)>::value, "");
			FLAT_ASSERT(object && callbackMethod);
			Callback* callback = new CallbackMethodImpl<PointerType>(object, callbackMethod);
			m_callbacks.push_back(callback);
		}

		template <typename Func>
		void on(Func callbackFunc)
		{
			//FLAT_ASSERT(callbackFunc); // no operator! for lambdas
			Callback* callback = new CallbackFunctionImpl<Func>(callbackFunc);
			m_callbacks.push_back(callback);
		}

		template <typename U>
		void off(U* object, void (U::*callbackMethod)(T...))
		{
			std::remove_if(m_callbacks.begin(), m_callbacks.end(),
				[object, callbackMethod](Callback* callback)
				{
					if (CallbackMethodImpl<U>* callbackMethodImpl = dynamic_cast<CallbackMethodImpl<U>*>(callback))
					{
						if (callbackMethodImpl->m_object == object && callbackMethodImpl->m_callbackMethod == callbackMethod)
						{
							delete callbackMethodImpl;
							return true;
						}
					}
					return false;
				}
			);
		}

		template <typename Func>
		void off(Func callbackFunc)
		{
			std::remove_if(m_callbacks.begin(), m_callbacks.end(),
				[callbackFunc](Callback* callback)
				{
					if (CallbackFunctionImpl<Func>* callbackFunctionImpl = dynamic_cast<CallbackFunctionImpl<Func>*>(callback))
					{
						if (callbackFunctionImpl->m_callbackFunc == callbackFunc)
						{
							delete callbackFunctionImpl;
							return true;
						}
					}
					return false;
				}
			);
		}

		template <typename U>
		void off(U* object)
		{
			std::remove_if(m_callbacks.begin(), m_callbacks.end(),
				[object](Callback* callback)
				{
					if (CallbackMethodImpl<U>* callbackMethodImpl = dynamic_cast<CallbackMethodImpl<U>*>(callback))
					{
						if (callbackMethodImpl->m_object == object)
						{
							delete callbackMethodImpl;
							return true;
						}
					}

					return false;
				}
			);
		}

		void off()
		{
			for (Callback* callback : m_callbacks)
				delete callback;

			m_callbacks.clear();
		}

	private:
		std::vector<Callback*> m_callbacks;
};

} // flat

#endif // FLAT_SLOT_H


