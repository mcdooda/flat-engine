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
		class CallbackImpl : public Callback
		{
			public:
				CallbackImpl(U* object, void (U::*callbackMethod)(T... params)) :
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
		class CallbackFuncImpl : public Callback
		{
			public:
				CallbackFuncImpl(Func callbackFunc) :
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
			disconnectAll();
		}

		void operator()(T... params)
		{
			for (Callback* callback : m_callbacks)
				(*callback)(params...);
		}

		template <typename U>
		void connect(U* object, void (std::remove_const<U>::type::*callbackMethod)(T...))
		{
			FLAT_ASSERT(object && callbackMethod);
			Callback* callback = new CallbackImpl<U>(object, callbackMethod);
			m_callbacks.push_back(callback);
		}

		template <typename Func>
		void connect(Func callbackFunc)
		{
			FLAT_ASSERT(callbackFunc);
			Callback* callback = new CallbackFuncImpl<Func>(callbackFunc);
			m_callbacks.push_back(callback);
		}

		template <typename U>
		void disconnect(U* object, void (U::*callbackMethod)(T...))
		{
			std::remove_if(m_callbacks.begin(), m_callbacks.end(),
				[object, callbackMethod](Callback* callback)
				{
					if (CallbackImpl<U>* callbackImpl = dynamic_cast<CallbackImpl<U>*>(callback))
					{
						if (callbackImpl->m_object == object && callbackImpl->m_callbackMethod == callbackMethod)
						{
							delete callbackImpl;
							return true;
						}
					}
					return false;
				}
			);
		}

		template <typename Func>
		void disconnect(Func callbackFunc)
		{
			std::remove_if(m_callbacks.begin(), m_callbacks.end(),
				[callbackFunc](Callback* callback)
				{
					if (CallbackFuncImpl<Func>* callbackFuncImpl = dynamic_cast<CallbackFuncImpl<Func>*>(callback))
					{
						if (callbackFuncImpl->m_callbackFunc == callbackFunc)
						{
							delete callbackFuncImpl;
							return true;
						}
					}
					return false;
				}
			);
		}

		template <typename U>
		void disconnectObject(U* object)
		{
			std::remove_if(m_callbacks.begin(), m_callbacks.end(),
				[object](Callback* callback)
				{
					if (CallbackImpl<U>* callbackImpl = dynamic_cast<CallbackImpl<U>*>(callback))
					{
						if (callbackImpl->m_object == object)
						{
							delete callbackImpl;
							return true;
						}
					}

					return false;
				}
			);
		}

		void disconnectAll()
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


