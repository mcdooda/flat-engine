#include "keyboard.h"

#include "context/inputcontext.h"

namespace flat
{
namespace input
{

Keyboard::Keyboard(const std::shared_ptr<context::InputContext>& globalInputContext) :
	m_globalInputContext(globalInputContext)
{
	
}

bool Keyboard::isPressed(Key key) const
{
	return m_globalInputContext->getKeyboardInputContext().isPressed(key);
}

bool Keyboard::isJustPressed(Key key) const
{
	return m_globalInputContext->getKeyboardInputContext().isJustPressed(key);
}

bool Keyboard::isJustReleased(Key key) const
{
	return m_globalInputContext->getKeyboardInputContext().isJustReleased(key);
}

void Keyboard::getPressedKeys(std::vector<Key>& pressedKeys) const
{
	return m_globalInputContext->getKeyboardInputContext().getPressedKeys(pressedKeys);
}

void Keyboard::getJustPressedKeys(std::vector<Key>& justPressedKeys) const
{
	return m_globalInputContext->getKeyboardInputContext().getJustPressedKeys(justPressedKeys);
}

void Keyboard::getJustReleasedKeys(std::vector<Key>& justReleasedKeys) const
{
	return m_globalInputContext->getKeyboardInputContext().getJustReleasedKeys(justReleasedKeys);
}

} // input
} // flat


