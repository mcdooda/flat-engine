#ifndef ENGINE_VIDEO_VIDEO_H
#define ENGINE_VIDEO_VIDEO_H

#include "window.h"
#include "color.h"

namespace engine
{
namespace video
{

class Video
{
	public:
		Video();
		~Video();
		
		void endFrame();
		
		void clear();
		void setClearColor(const Color& color);

	public:
		Window* window;
};

} // video
} // engine

#endif // ENGINE_VIDEO_VIDEO_H


