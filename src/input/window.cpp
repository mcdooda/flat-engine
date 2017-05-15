#include "window.h"

#include "context/inputcontext.h"

namespace flat
{
namespace input
{

Window::Window(const std::shared_ptr<context::InputContext>& globalInputContext) :
	m_globalInputContext(globalInputContext)
{

}

bool Window::isClosed() const
{
	return m_globalInputContext->getWindowInputContext().isClosed();
}

bool Window::isResized() const
{
	return m_globalInputContext->getWindowInputContext().isResized();
}

} // input
} // flat


