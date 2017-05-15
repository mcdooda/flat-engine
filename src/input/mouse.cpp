#include "mouse.h"

#include "context/inputcontext.h"

namespace flat
{
namespace input
{

Mouse::Mouse(const std::shared_ptr<context::InputContext>& globalInputContext) :
	m_globalInputContext(globalInputContext)
{
}

bool Mouse::isPressed(int button) const
{
	return m_globalInputContext->getMouseInputContext().isPressed(button);
}

bool Mouse::isJustPressed(int button) const
{
	return m_globalInputContext->getMouseInputContext().isJustPressed(button);
}

bool Mouse::isJustReleased(int button) const
{
	return m_globalInputContext->getMouseInputContext().isJustReleased(button);
}

bool Mouse::isJustDoubleClicked(int button) const
{
	return m_globalInputContext->getMouseInputContext().isJustDoubleClicked(button);
}

bool Mouse::justMoved() const
{
	return m_globalInputContext->getMouseInputContext().justMoved();
}

const Vector2& Mouse::getPosition() const
{
	return m_globalInputContext->getMouseInputContext().getPosition();
}

bool Mouse::wheelJustMoved() const
{
	return m_globalInputContext->getMouseInputContext().wheelJustMoved();
}

const Vector2& Mouse::getWheelMove() const
{
	return m_globalInputContext->getMouseInputContext().getWheelMove();
}

} // input
} // flat


