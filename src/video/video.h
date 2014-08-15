#ifndef FLAT_VIDEO_VIDEO_H
#define FLAT_VIDEO_VIDEO_H

#include "window.h"
#include "color.h"

namespace flat
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
} // flat

#endif // FLAT_VIDEO_VIDEO_H


