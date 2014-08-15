#ifndef FLAT_INPUT_INPUT_H
#define FLAT_INPUT_INPUT_H

#include "keyboard.h"
#include "mouse.h"
#include "window.h"
#include "../video/video.h"

namespace flat
{
namespace input
{

class Input
{
	public:
		Input(video::Window* videoWindow);
		~Input();
		
		void poll();
		void clearAll();
		
	public:
		Keyboard* keyboard;
		Mouse* mouse;
		Window* window;
};

} // input
} // flat

#endif // FLAT_INPUT_INPUT_H


