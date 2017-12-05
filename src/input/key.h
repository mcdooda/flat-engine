#ifndef FLAT_INPUT_KEY_H
#define FLAT_INPUT_KEY_H

#include <SDL2/SDL.h>

#define K(k) SDL_SCANCODE_##k
#define KMOD(k) KMOD_##k

namespace flat
{
namespace input
{

using Key = SDL_Scancode;
using KeyMod = SDL_Keymod;

} // input
} // flat

#endif // FLAT_INPUT_KEY_H


