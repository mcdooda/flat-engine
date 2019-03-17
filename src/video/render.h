#ifndef FLAT_VIDEO_RENDER_H
#define FLAT_VIDEO_RENDER_H

#include <GL/glew.h>

#include "video/program.h"

#include "geometry/rectangle.h"

namespace flat
{
namespace video
{

class Render : public Program
{
	public:
		Render();
		virtual ~Render();
		
		void load(const std::string& fragmentShader, const std::string& vertexShader);
		
		void draw();
		
	private:
		Attribute m_positionAttribute;
		Attribute m_uvAttribute;
		
		static geometry::Rectangle screenRectangle;
};

} // video
} // flat

#endif // FLAT_VIDEO_RENDER_H


