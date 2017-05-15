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
	cleanTopContexts();

	FLAT_ASSERT(m_globalInputContext.get() != nullptr);
	context::InputContext& globalInputContext = *m_globalInputContext.get();
	context::KeyboardInputContext& keyboardInputContext = globalInputContext.getKeyboardInputContext();
	context::MouseInputContext& mouseInputContext = globalInputContext.getMouseInputContext();
	context::WindowInputContext& windowInputContext = globalInputContext.getWindowInputContext();

	keyboardInputContext.clearEvents();
	mouseInputContext.clearEvents();
	windowInputContext.clearEvents();

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		keyboardInputContext.addEvent(event);
		mouseInputContext.addEvent(event);
		windowInputContext.addEvent(event);
	}

	// copy the global context into the top context
	FLAT_ASSERT(!m_inputContexts.back().expired());
	context::InputContext& topInputContext = *m_inputContexts.back().lock().get();
	if (&topInputContext != &globalInputContext)
	{
		topInputContext = globalInputContext;
	}
}

void Input::clearEvents()
{
	SDL_Event e;
	while (SDL_PollEvent(&e));

	for (std::weak_ptr<context::InputContext>& inputContextWeakPtr : m_inputContexts)
	{
		context::InputContext* inputContext = inputContextWeakPtr.lock().get();
		if (inputContext != nullptr)
		{
			inputContext->clearEvents();
		}
	}
}

void Input::pushContext(const std::shared_ptr<context::InputContext>& inputContext)
{
	FLAT_ASSERT(inputContext.get() != nullptr);
	inputContext->clearEvents();
	m_inputContexts.push_back(inputContext);
}

void Input::popContext(const std::shared_ptr<context::InputContext>& inputContext)
{
	FLAT_ASSERT(inputContext == m_inputContexts.back().lock());
	m_inputContexts.pop_back();
}

void Input::cleanTopContexts()
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

} // input
} // flat


