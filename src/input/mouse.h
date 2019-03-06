#ifndef FLAT_INPUT_MOUSE_H
#define FLAT_INPUT_MOUSE_H

#include <array>
#include <SDL2/SDL.h>

#include "misc/vector.h"
#include "video/video.h"
#include "video/view.h"

#define M(m) SDL_BUTTON_##m

namespace flat
{
namespace input
{
namespace context
{
class InputContext;
}

class Mouse
{
	public:
		Mouse() = delete;
		Mouse(const Mouse&) = delete;
		Mouse(Mouse&&) = delete;
		Mouse(const std::shared_ptr<context::InputContext>& globalInputContext);
		~Mouse() = default;
		
		bool isPressed(int button) const;
		bool isJustPressed(int button) const;
		bool isJustReleased(int button) const;
		bool isJustDoubleClicked(int button) const;

		bool justMoved() const;
		const Vector2& getPosition() const;
		
		bool wheelJustMoved() const;
		const Vector2& getWheelMove() const;
		
	private:
		std::shared_ptr<context::InputContext> m_globalInputContext;
};

} // input
} // flat

#endif // FLAT_INPUT_MOUSE_H


