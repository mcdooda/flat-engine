#ifndef FLAT_INPUT_INPUT_H
#define FLAT_INPUT_INPUT_H

#include <memory>
#include <vector>

#include "keyboard.h"
#include "mouse.h"
#include "window.h"

#include "context/inputcontext.h"

namespace flat
{
class Flat;
namespace input
{
namespace context
{
class InputContext;
}

class Input
{
	public:
		Input() = delete;
		Input(const Input&) = delete;
		Input(Input&&) = delete;
		Input(Flat& flat);
		~Input() = default;
		Input& operator=(const Input&) = delete;
		
		void pollEvents();
		void clearFrameEvents();

		void pushContext(const std::shared_ptr<context::InputContext>& inputContext);
		void popContext(const std::shared_ptr<context::InputContext>& inputContext);
		
	public:
		std::unique_ptr<Keyboard> keyboard;
		std::unique_ptr<Mouse> mouse;
		std::unique_ptr<Window> window;

	private:
		void cleanTopContexts();

	private:
		std::shared_ptr<context::InputContext> m_globalInputContext;

		// used as a stack but we need an iterable container
		std::vector<std::weak_ptr<context::InputContext>> m_inputContexts;
};

} // input
} // flat

#endif // FLAT_INPUT_INPUT_H


