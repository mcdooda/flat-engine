#ifndef FLAT_VIDEO_PASS_H
#define FLAT_VIDEO_PASS_H

#include <GL/glew.h>

#include "video/program.h"
#include "video/framebuffer.h"

namespace flat
{
namespace video
{

class Pass : public Program
{
	public:
		Pass();
		virtual ~Pass();
		
		void load(FrameBuffer* frameBuffer, const std::string& fragmentShader, const std::string& vertexShader);
		
		void use();
		
	private:
		FrameBuffer* m_frameBuffer;
};

} // video
} // flat

#endif // FLAT_VIDEO_PASS_H


