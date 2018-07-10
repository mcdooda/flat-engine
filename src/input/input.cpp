#include "input.h"

#include "context/keyboardinputcontext.h"
#include "context/mouseinputcontext.h"
#include "context/windowinputcontext.h"

namespace flat
{
namespace input
{

Input::Input(Flat& flat)
{
	m_globalInputContext = std::make_shared<context::InputContext>(flat);
	m_inputContexts.push_back(m_globalInputContext);

	keyboard = std::make_unique<Keyboard>(m_globalInputContext);
	mouse = std::make_unique<Mouse>(m_globalInputContext);
	window = std::make_unique<Window>(m_globalInputContext);
}

void Input::pollEvents()
{
	cleanTopExpiredContexts();

	FLAT_ASSERT(!m_inputContexts.back().expired());
	context::InputContext& topInputContext = *m_inputContexts.back().lock().get();

	topInputContext.clearFrameEvents();

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		topInputContext.addEvent(event);
	}

	// copy the top context into the global context
	//!\ warning: does NOT trigger key press/release slots on the global context!
	FLAT_ASSERT(m_globalInputContext.get() != nullptr);
	context::InputContext& globalInputContext = *m_globalInputContext.get();
	if (&topInputContext != &globalInputContext)
	{
		globalInputContext = topInputContext;
	}
}

void Input::clearFrameEvents()
{
	SDL_Event e;
	while (SDL_PollEvent(&e));

	for (std::weak_ptr<context::InputContext>& inputContextWeakPtr : m_inputContexts)
	{
		context::InputContext* inputContext = inputContextWeakPtr.lock().get();
		if (inputContext != nullptr)
		{
			inputContext->clearFrameEvents();
		}
	}
}

void Input::pushContext(const std::shared_ptr<context::InputContext>& inputContext)
{
	FLAT_ASSERT(inputContext != nullptr);
	FLAT_ASSERT(m_inputContexts.size() > 0);
	inputContext->clearAllEvents();
	std::shared_ptr<context::InputContext> currentTopContext = m_inputContexts.back().lock();
	FLAT_ASSERT(currentTopContext != nullptr);
	inputContext->copyStateFrom(*currentTopContext.get());
	clearTopContext();
	m_inputContexts.push_back(inputContext);
}

void Input::popContext(const std::shared_ptr<context::InputContext>& inputContext)
{
	FLAT_ASSERT(inputContext != nullptr && inputContext == m_inputContexts.back().lock());
	inputContext->clearAllEvents();
	m_inputContexts.pop_back();
	clearTopContext();
	FLAT_ASSERT_MSG(!m_inputContexts.empty(), "Cannot pop the global input context!");
}

void Input::cleanTopExpiredContexts()
{
	FLAT_ASSERT(!m_inputContexts.empty() && !m_inputContexts[0].expired());

	if (m_inputContexts.back().expired())
	{
		int i = static_cast<int>(m_inputContexts.size() - 1);
		while (i > 0 && m_inputContexts[i].expired())
		{
			--i;
		}

		m_inputContexts.erase(m_inputContexts.begin() + i + 1, m_inputContexts.end());
	}
}

void Input::clearTopContext()
{
	// clear current context if we have one and it is not the global one
	if (m_inputContexts.size() > 1)
	{
		context::InputContext* topInputContext = m_inputContexts.back().lock().get();
		if (topInputContext != nullptr)
		{
			topInputContext->clearAllEvents();
		}
	}
}

} // input
} // flat


