#ifndef ENGINE_VIDEO_RENDER_H
#define ENGINE_VIDEO_RENDER_H

#include <GL/glew.h>
#include "program.h"
#include "../geometry/rectangle.h"

namespace engine
{
namespace video
{

class Render : public Program
{
	public:
		Render();
		virtual ~Render();
		
		void load(std::string fragmentShader, std::string vertexShader);
		
		void draw();
		
	private:
		Attribute m_positionAttribute;
		Attribute m_uvAttribute;
		
		static geometry::Rectangle screenRectangle;
};

} // video
} // engine

#endif // ENGINE_VIDEO_RENDER_H


