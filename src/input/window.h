#ifndef FLAT_INPUT_WINDOW_H
#define FLAT_INPUT_WINDOW_H

#include <memory>

namespace flat
{
namespace input
{
namespace context
{
class InputContext;
}

class Window
{
	public:
		Window() = delete;
		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window(const std::shared_ptr<context::InputContext>& globalInputContext);
		~Window() = default;
		
		bool isClosed() const;
		bool isResized() const;
		
	private:
		std::shared_ptr<context::InputContext> m_globalInputContext;
};

} // input
} // flat

#endif // FLAT_INPUT_WINDOW_H


