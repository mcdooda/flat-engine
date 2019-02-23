#ifndef FLAT_INPUT_GAMEPADBUTTONS_H
#define FLAT_INPUT_GAMEPADBUTTONS_H

#include <SDL2/SDL.h>

#define B(b) SDL_CONTROLLER_BUTTON_##b
#define A(a) SDL_CONTROLLER_AXIS_##a

namespace flat
{
namespace input
{

using GamepadIndex = int;
using GamepadButton = SDL_GameControllerButton;
using GamepadAxis = SDL_GameControllerAxis;

} // input
} // flat

#endif // FLAT_INPUT_GAMEPADBUTTONS_H


